#pragma once

#include <array>
#include <atomic>
#include <chrono>
#include <cstdint>
#include <string>
#include <vector>

namespace RuntimeStats
{
	enum class ThreadId : size_t
	{
		PID,
		Core,
		KeyState,
		Actors,
		Players,
		FogPlayers,
		Vehicles,
		AimBot,
		Camera,
		Items,
		Projects,
		Radar,
		Recoil,
		Overlay,
		Count
	};

	inline constexpr size_t ThreadCount = static_cast<size_t>(ThreadId::Count);
	inline constexpr std::array<const char*, ThreadCount> ThreadNames = {
		"PID",
		"Core",
		"KeyState",
		"Actors",
		"Players",
		"FogPlayers",
		"Vehicles",
		"AimBot",
		"Camera",
		"Items",
		"Projects",
		"Radar",
		"Recoil",
		"Overlay"
	};

	struct AtomicThreadStats
	{
		std::atomic<uint64_t> TickCount{ 0 };
		std::atomic<uint64_t> LastDurationUs{ 0 };
		std::atomic<uint64_t> MaxDurationUs{ 0 };
		std::atomic<uint64_t> LastSeenMs{ 0 };
		std::atomic<uint64_t> ErrorCount{ 0 };
		std::atomic<uint32_t> LastItemCount{ 0 };
	};

	struct ThreadSnapshot
	{
		std::string Name;
		uint64_t TickCount = 0;
		uint64_t LastDurationUs = 0;
		uint64_t MaxDurationUs = 0;
		uint64_t LastSeenAgeMs = 0;
		uint64_t ErrorCount = 0;
		uint32_t LastItemCount = 0;
	};

	inline std::array<AtomicThreadStats, ThreadCount> Stats{};

	inline std::chrono::steady_clock::time_point Now()
	{
		return std::chrono::steady_clock::now();
	}

	inline uint64_t NowMs()
	{
		return static_cast<uint64_t>(std::chrono::duration_cast<std::chrono::milliseconds>(
			Now().time_since_epoch()).count());
	}

	inline void Record(ThreadId id, const std::chrono::steady_clock::time_point start, const uint32_t itemCount = 0)
	{
		const auto end = Now();
		const auto durationUs = static_cast<uint64_t>(std::chrono::duration_cast<std::chrono::microseconds>(end - start).count());
		const auto index = static_cast<size_t>(id);
		if (index >= ThreadCount)
		{
			return;
		}

		auto& stat = Stats[index];
		stat.TickCount.fetch_add(1, std::memory_order_relaxed);
		stat.LastDurationUs.store(durationUs, std::memory_order_relaxed);
		stat.LastSeenMs.store(NowMs(), std::memory_order_relaxed);
		stat.LastItemCount.store(itemCount, std::memory_order_relaxed);

		uint64_t currentMax = stat.MaxDurationUs.load(std::memory_order_relaxed);
		while (durationUs > currentMax &&
			!stat.MaxDurationUs.compare_exchange_weak(currentMax, durationUs, std::memory_order_relaxed))
		{
		}
	}

	inline void AddError(ThreadId id)
	{
		const auto index = static_cast<size_t>(id);
		if (index < ThreadCount)
		{
			Stats[index].ErrorCount.fetch_add(1, std::memory_order_relaxed);
		}
	}

	class ScopedRecord
	{
	public:
		explicit ScopedRecord(ThreadId id, uint32_t itemCount = 0)
			: Id(id), Start(Now()), ItemCount(itemCount)
		{
		}

		~ScopedRecord()
		{
			if (Active)
			{
				Record(Id, Start, ItemCount);
			}
		}

		void SetItemCount(uint32_t itemCount)
		{
			ItemCount = itemCount;
		}

		void Dismiss()
		{
			Active = false;
		}

	private:
		ThreadId Id;
		std::chrono::steady_clock::time_point Start;
		uint32_t ItemCount = 0;
		bool Active = true;
	};

	inline std::vector<ThreadSnapshot> Snapshot()
	{
		std::vector<ThreadSnapshot> snapshots;
		snapshots.reserve(ThreadCount);
		const auto nowMs = NowMs();

		for (size_t i = 0; i < ThreadCount; ++i)
		{
			const auto& stat = Stats[i];
			ThreadSnapshot snapshot;
			snapshot.Name = ThreadNames[i];
			snapshot.TickCount = stat.TickCount.load(std::memory_order_relaxed);
			snapshot.LastDurationUs = stat.LastDurationUs.load(std::memory_order_relaxed);
			snapshot.MaxDurationUs = stat.MaxDurationUs.load(std::memory_order_relaxed);
			const auto lastSeenMs = stat.LastSeenMs.load(std::memory_order_relaxed);
			snapshot.LastSeenAgeMs = lastSeenMs > 0 && nowMs >= lastSeenMs ? nowMs - lastSeenMs : 0;
			snapshot.ErrorCount = stat.ErrorCount.load(std::memory_order_relaxed);
			snapshot.LastItemCount = stat.LastItemCount.load(std::memory_order_relaxed);
			snapshots.emplace_back(snapshot);
		}

		return snapshots;
	}
}
