#include <WS2tcpip.h>
#include <iostream>
#include <regex>
#include <unordered_map>
#include "cJSON/cJSON.h"
#include <future>
#include <string>
#include <vector>
#include <wininet.h>
#include <map>

#include <Common/Data.h>

#pragma comment(lib, "wininet.lib")

class Rank
{
public:
    std::string sendGetRequestWithWininet(const std::string& host, int port, const std::string& path) {
        (void)host;
        (void)port;
        (void)path;
        return "";
    }

    std::string sendPostRequestWithWininet(const std::string& host, int port, const std::string& path, const std::string& data) {
        (void)host;
        (void)port;
        (void)path;
        (void)data;
        return "";
    }

    std::string extractOrReturnOriginal(const std::string& input) {
        std::regex pattern(R"(\[.*\]\s*(\S+))");
        std::smatch matches;

        if (std::regex_search(input, matches, pattern) && matches.size() > 1) {
            std::string result = matches[1].str();
            result.erase(remove(result.begin(), result.end(), ' '), result.end());
            return result;
        }
        else {
            return input;
        }
    }

    void fetcahwebget(const std::string& name)
    {
        std::string url = "/user/" + name;
        sendGetRequestWithWininet("pubg.op.gg", 80, url);
    }

    std::string extractSpecificString(const std::string& response) {
        std::regex pattern(R"(<div[^>]*\bid\s*=\s*\"userNickname\"[^>]*\bdata-user_id\s*=\s*\"([^\"]+)\")");
        std::smatch matches;

        if (std::regex_search(response, matches, pattern) && matches.size() > 1) {
            return matches[1].str();
        }
        return "";
    }

    std::string fetchWebGet(const std::string& name) {
        std::string url = "/user/" + name;
        return sendGetRequestWithWininet("pubg.op.gg", 80, url);
    }

    std::string fetchUserRankedStats(const std::string& userId) {
        std::string url = "/api/users/" + userId + GameData.Config.ESP.RankModeUrl;
        return sendGetRequestWithWininet("pubg.op.gg", 80, url);
    }

    void concurrentRequests(const std::vector<std::string>& names, std::unordered_map<std::string, bool>& QuerySuccess, std::unordered_map<std::string, int>& QueryCount) {

        std::map<std::string, std::future<std::string>> futures;
        std::mutex queryMutex;

        for (const auto& name : names) {

            if (QuerySuccess[name])
            {
                continue;
            }

            if (QueryCount[name] >= 2)
            {
                continue;
            }

            futures[name] = std::async(std::launch::async, [this, name]() {
                std::string url = "/user/" + name;
                return this->sendGetRequestWithWininet("pubg.op.gg", 80, url);
                });

            Sleep(2000);
        }

        for (auto& [name, future] : futures) {

            if (QuerySuccess[name])
            {
                continue;
            }

            if (QueryCount[name] >= 2)
            {
                continue;
            }

            std::string response = future.get();
            std::string extractedString = extractSpecificString(response);
            Utils::Log(1, "%s", Utils::StringToUTF8(response).c_str());



            if (!extractedString.empty()) {

                std::string rankedStatsResponse = fetchUserRankedStats(extractedString);

                cJSON* jsonRoot = cJSON_Parse(rankedStatsResponse.c_str());
                if (jsonRoot) {
                    PlayerRankList RankList;
                    {
                        std::lock_guard<std::mutex> lock(queryMutex);
                        QuerySuccess[name] = true;
                    }

                    if (GameData.Config.PlayerList.RankMode == 1)
                    {
                        cJSON* tier = cJSON_GetObjectItem(jsonRoot, "tier");
                        if (tier) {
                            cJSON* title = cJSON_GetObjectItem(tier, "title");
                            if (title) {
                                RankList.TPP.Tier = title->valuestring;
                                std::cout << "RankList.SquadFPP.Tier: " << RankList.TPP.Tier << std::endl;
                            }
                        }

                        cJSON* stats = cJSON_GetObjectItem(jsonRoot, "stats");
                        if (stats) {
                            int kills = 0;
                            int deaths = 0;

                            int matches_cnt = 0;
                            int win_matches_cnt = 0;

                            cJSON* rank_points = cJSON_GetObjectItem(stats, "rank_points");
                            if (rank_points) {
                                RankList.TPP.RankPoint = rank_points->valueint;
                                std::cout << "RankList.SquadFPP.RankPoint: " << RankList.TPP.RankPoint << std::endl;
                            }

                            cJSON* killsSum = cJSON_GetObjectItem(stats, "kills_sum");
                            if (killsSum) {
                                kills = killsSum->valueint;
                            }

                            cJSON* deathsSum = cJSON_GetObjectItem(stats, "deaths_sum");
                            if (deathsSum) {
                                deaths = deathsSum->valueint;
                            }

                            cJSON* matchesSum = cJSON_GetObjectItem(stats, "matches_cnt");
                            if (matchesSum) {
                                matches_cnt = matchesSum->valueint;
                            }

                            cJSON* win_matchesSum = cJSON_GetObjectItem(stats, "win_matches_cnt");
                            if (win_matchesSum) {
                                win_matches_cnt = win_matchesSum->valueint;
                            }

                            RankList.TPP.WinRatio = static_cast<double>(matches_cnt) / win_matches_cnt;

                            RankList.TPP.KDA = static_cast<double>(kills) / deaths;
                            std::cout << "RankList.SquadFPP.KDA: " << RankList.TPP.KDA << std::endl;
                            std::cout << "RankList.WinRatio: \n" << RankList.TPP.WinRatio << std::endl;
                        }
                    }
                    else if (GameData.Config.PlayerList.RankMode == 2)
                    {
                        cJSON* tier = cJSON_GetObjectItem(jsonRoot, "tier");
                        if (tier) {
                            cJSON* title = cJSON_GetObjectItem(tier, "title");
                            if (title) {
                                RankList.SquadTPP.Tier = title->valuestring;
                                std::cout << "RankList.SquadFPP.Tier: " << RankList.SquadTPP.Tier << std::endl;
                            }
                        }

                        cJSON* stats = cJSON_GetObjectItem(jsonRoot, "stats");
                        if (stats) {
                            int kills = 0;
                            int deaths = 0;

                            int matches_cnt = 0;
                            int win_matches_cnt = 0;

                            cJSON* rank_points = cJSON_GetObjectItem(stats, "rank_points");
                            if (rank_points) {
                                RankList.SquadTPP.RankPoint = rank_points->valueint;
                                std::cout << "RankList.SquadFPP.RankPoint: " << RankList.SquadTPP.RankPoint << std::endl;
                            }

                            cJSON* killsSum = cJSON_GetObjectItem(stats, "kills_sum");
                            if (killsSum) {
                                kills = killsSum->valueint;
                            }

                            cJSON* deathsSum = cJSON_GetObjectItem(stats, "deaths_sum");
                            if (deathsSum) {
                                deaths = deathsSum->valueint;
                            }

                            cJSON* matchesSum = cJSON_GetObjectItem(stats, "matches_cnt");
                            if (matchesSum) {
                                matches_cnt = matchesSum->valueint;
                            }

                            cJSON* win_matchesSum = cJSON_GetObjectItem(stats, "win_matches_cnt");
                            if (win_matchesSum) {
                                win_matches_cnt = win_matchesSum->valueint;
                            }

                            RankList.SquadTPP.WinRatio = static_cast<double>(matches_cnt) / win_matches_cnt;

                            RankList.SquadTPP.KDA = static_cast<double>(kills) / deaths;
                            std::cout << "RankList.SquadFPP.KDA: " << RankList.SquadTPP.KDA << std::endl;
                            std::cout << "RankList.WinRatio: \n" << RankList.SquadTPP.WinRatio << std::endl;
                        }
                    }
                    else if (GameData.Config.PlayerList.RankMode == 3)
                    {
                        cJSON* tier = cJSON_GetObjectItem(jsonRoot, "tier");
                        if (tier) {
                            cJSON* title = cJSON_GetObjectItem(tier, "title");
                            if (title) {
                                RankList.FPP.Tier = title->valuestring;
                                std::cout << "RankList.SquadFPP.Tier: " << RankList.FPP.Tier << std::endl;
                            }
                        }

                        cJSON* stats = cJSON_GetObjectItem(jsonRoot, "stats");
                        if (stats) {
                            int kills = 0;
                            int deaths = 0;

                            int matches_cnt = 0;
                            int win_matches_cnt = 0;

                            cJSON* rank_points = cJSON_GetObjectItem(stats, "rank_points");
                            if (rank_points) {
                                RankList.FPP.RankPoint = rank_points->valueint;
                                std::cout << "RankList.SquadFPP.RankPoint: " << RankList.FPP.RankPoint << std::endl;
                            }

                            cJSON* killsSum = cJSON_GetObjectItem(stats, "kills_sum");
                            if (killsSum) {
                                kills = killsSum->valueint;
                            }

                            cJSON* deathsSum = cJSON_GetObjectItem(stats, "deaths_sum");
                            if (deathsSum) {
                                deaths = deathsSum->valueint;
                            }

                            cJSON* matchesSum = cJSON_GetObjectItem(stats, "matches_cnt");
                            if (matchesSum) {
                                matches_cnt = matchesSum->valueint;
                            }

                            cJSON* win_matchesSum = cJSON_GetObjectItem(stats, "win_matches_cnt");
                            if (win_matchesSum) {
                                win_matches_cnt = win_matchesSum->valueint;
                            }

                            RankList.FPP.WinRatio = static_cast<double>(matches_cnt) / win_matches_cnt;

                            RankList.FPP.KDA = static_cast<double>(kills) / deaths;
                            std::cout << "RankList.SquadFPP.KDA: " << RankList.FPP.KDA << std::endl;
                            std::cout << "RankList.WinRatio: " << RankList.FPP.WinRatio << std::endl;
                        }
                    }
                    else if (GameData.Config.PlayerList.RankMode == 4)
                    {
                        cJSON* tier = cJSON_GetObjectItem(jsonRoot, "tier");
                        if (tier) {
                            cJSON* title = cJSON_GetObjectItem(tier, "title");
                            if (title) {
                                RankList.SquadFPP.Tier = title->valuestring;
                                std::cout << "RankList.SquadFPP.Tier: " << RankList.SquadFPP.Tier << std::endl;
                            }
                        }

                        cJSON* stats = cJSON_GetObjectItem(jsonRoot, "stats");
                        if (stats) {
                            int kills = 0;
                            int deaths = 0;

                            int matches_cnt = 0;
                            int win_matches_cnt = 0;

                            cJSON* rank_points = cJSON_GetObjectItem(stats, "rank_points");
                            if (rank_points) {
                                RankList.SquadFPP.RankPoint = rank_points->valueint;
                                std::cout << "RankList.SquadFPP.RankPoint: " << RankList.SquadFPP.RankPoint << std::endl;
                            }

                            cJSON* killsSum = cJSON_GetObjectItem(stats, "kills_sum");
                            if (killsSum) {
                                kills = killsSum->valueint;
                            }

                            cJSON* deathsSum = cJSON_GetObjectItem(stats, "deaths_sum");
                            if (deathsSum) {
                                deaths = deathsSum->valueint;
                            }

                            cJSON* matchesSum = cJSON_GetObjectItem(stats, "matches_cnt");
                            if (matchesSum) {
                                matches_cnt = matchesSum->valueint;
                            }

                            cJSON* win_matchesSum = cJSON_GetObjectItem(stats, "win_matches_cnt");
                            if (win_matchesSum) {
                                win_matches_cnt = win_matchesSum->valueint;
                            }

                            RankList.SquadFPP.WinRatio = static_cast<double>(matches_cnt) / win_matches_cnt;

                            RankList.SquadFPP.KDA = static_cast<double>(kills) / deaths;
                            std::cout << "RankList.SquadFPP.KDA: " << RankList.SquadFPP.KDA << std::endl;
                            std::cout << "RankList.WinRatio:\n " << RankList.SquadFPP.WinRatio << std::endl;
                        }
                    }
                    Data::SetPlayerRankListsItem(name, RankList);
                    cJSON_Delete(jsonRoot);
                }
                else {
                    {
                        std::lock_guard<std::mutex> lock(queryMutex);
                        QueryCount[name]++;
                    }
                    std::cerr << "Failed to 2 JSON response." << std::endl;
                }
            }
            else {
                {
                    std::lock_guard<std::mutex> lock(queryMutex);
                    QueryCount[name]++;
                }

                std::cerr << "User opggID not found." << std::endl;
            }
        }
    }

    std::pair<std::string, PlayerRankList> fetchPlayerRank(const std::string& name) {
        std::string url = "/user/" + name;
        std::string response = sendGetRequestWithWininet("pubg.op.gg", 80, url);

        std::string extractedString = extractSpecificString(response);

        //std::cout << "OpggID: " << extractedString << std::endl;

        PlayerRankList playerRankList;
        if (!extractedString.empty()) {
            std::string rankedStatsResponse = fetchUserRankedStats(extractedString);

            cJSON* jsonRoot = cJSON_Parse(rankedStatsResponse.c_str());
            if (jsonRoot) {
                int kills = 0;
                int deaths = 0;
                int rank = 0;

                cJSON* tier = cJSON_GetObjectItem(jsonRoot, "tier");
                if (tier) {
                    cJSON* title = cJSON_GetObjectItem(tier, "title");
                    if (title) {
                        playerRankList.SquadFPP.Tier = title->valuestring;
                    }
                }

                cJSON* stats = cJSON_GetObjectItem(jsonRoot, "stats");
                if (stats) {
                    cJSON* rank_points = cJSON_GetObjectItem(stats, "rank_points");
                    if (rank_points) {
                        playerRankList.SquadFPP.RankPoint = rank_points->valueint;
                    }

                    cJSON* killsSum = cJSON_GetObjectItem(stats, "kills_sum");
                    if (killsSum) {
                        kills = killsSum->valueint;
                    }

                    cJSON* deathsSum = cJSON_GetObjectItem(stats, "deaths_sum");
                    if (deathsSum) {
                        deaths = deathsSum->valueint;
                    }
                    playerRankList.SquadFPP.KDA = static_cast<double>(kills) / deaths;
                }

                cJSON_Delete(jsonRoot);
            }
        }

        return { name, playerRankList };
    }

    void concurrentRequestsPaid(const std::vector<std::string>& names, std::unordered_map<std::string, bool>& QuerySuccess, std::unordered_map<std::string, int>& QueryCount) {
        std::map<std::string, std::future<std::string>> futures;
        std::mutex queryMutex;

        for (const auto& name : names) {

            // 妫€鏌ヨ鐜╁鏄惁宸茬粡鎴愬姛鏌ヨ杩?            if (QuerySuccess[name])
            {
                continue;
            }

            if (QueryCount[name] >= 2)
            {
                continue;
            }
            //printf("%s/n", name);
            futures[name] = std::async(std::launch::async, [this, name]() {
                std::string url = "/user/" + name;
                return this->extractPaidInterface(name, "TPP");
                });
        }




        std::vector<std::future<void>> futures_list;  // 鐢ㄤ簬瀛樺偍鎵€鏈夌殑寮傛浠诲姟

        for (auto& [name, future] : futures) {
            // 妫€鏌ヨ鐜╁鏄惁宸茬粡鎴愬姛鏌ヨ杩?            if (QuerySuccess[name]) {
                continue;
            }

            if (QueryCount[name] >= 2) {
                continue;
            }

            // 灏嗘瘡涓换鍔″紓姝ュ寲
            futures_list.push_back(std::async(std::launch::async, [&] {
                std::string response = future.get();
                if (!response.empty()) {
                    PlayerRankList RankList;

                    {
                        std::lock_guard<std::mutex> lock(queryMutex);
                        QuerySuccess[name] = true;
                    }

                    // 瑙ｆ瀽 JSON 鏁版嵁
                    cJSON* root = cJSON_Parse(response.c_str());
                    if (root == nullptr) {
                        return;
                    }
                    // std::cout << "Root JSON: " << cJSON_Print(root) << std::endl;
                     // 鑾峰彇 "data" 瀵硅薄
                    cJSON* data = cJSON_GetObjectItemCaseSensitive(root, "data");
                    if (data == nullptr) {
                        std::cout << "Root: " << root << std::endl;
                        cJSON_Delete(root);
                        return;
                    }

                    cJSON* attributes = cJSON_GetObjectItemCaseSensitive(data, "attributes");
                    if (attributes == nullptr) {
                        cJSON_Delete(data);
                        return;
                    }

                    cJSON* rankedGameModeStats = cJSON_GetObjectItemCaseSensitive(attributes, "rankedGameModeStats");
                    if (rankedGameModeStats == nullptr) {
                        cJSON_Delete(attributes);
                        return;
                    }
                    RankList.SquadFPP.Tier = Utils::StringToUTF8("鏈畾绾?);

                    // 澶勭悊 SquadFPP
                    cJSON* squad_fpp = cJSON_GetObjectItemCaseSensitive(rankedGameModeStats, "squad-fpp");
                    if (squad_fpp != nullptr) {
                        // 瑙ｆ瀽 "currentTier"
                        cJSON* currentTier = cJSON_GetObjectItemCaseSensitive(squad_fpp, "currentTier");
                        if (currentTier != nullptr) {
                            cJSON* tier = cJSON_GetObjectItemCaseSensitive(currentTier, "tier");
                            cJSON* subTier = cJSON_GetObjectItemCaseSensitive(currentTier, "subTier");

                            if (tier && subTier) {
                                // 鎷兼帴 "tier" 鍜?"subTier"
                                RankList.SquadFPP.Tier = std::string(tier->valuestring) + " " + subTier->valuestring;
                            }

                        }

                        // 鑾峰彇鍏朵粬瀛楁
                        cJSON* currentRankPoint = cJSON_GetObjectItemCaseSensitive(squad_fpp, "currentRankPoint");
                        if (currentRankPoint != nullptr) {
                            RankList.SquadFPP.RankPoint = currentRankPoint->valueint;
                        }

                        cJSON* kda = cJSON_GetObjectItemCaseSensitive(squad_fpp, "kda");
                        if (kda != nullptr) {
                            RankList.SquadFPP.KDA = static_cast<float>(kda->valuedouble);
                        }

                        cJSON* winRatio = cJSON_GetObjectItemCaseSensitive(squad_fpp, "winRatio");
                        if (winRatio != nullptr) {
                            RankList.SquadFPP.WinRatio = static_cast<float>(winRatio->valuedouble);
                        }
                    }
                    RankList.SquadTPP.Tier = Utils::StringToUTF8("鏈畾绾?);
                    // 澶勭悊 SquadTPP
                    cJSON* squad = cJSON_GetObjectItemCaseSensitive(rankedGameModeStats, "squad");
                    if (squad != nullptr) {
                        // 瑙ｆ瀽 "currentTier"
                        cJSON* currentTier = cJSON_GetObjectItemCaseSensitive(squad, "currentTier");
                        if (currentTier != nullptr) {
                            cJSON* tier = cJSON_GetObjectItemCaseSensitive(currentTier, "tier");
                            cJSON* subTier = cJSON_GetObjectItemCaseSensitive(currentTier, "subTier");

                            if (tier && subTier) {
                                // 鎷兼帴 "tier" 鍜?"subTier"
                                RankList.SquadTPP.Tier = std::string(tier->valuestring) + " " + subTier->valuestring;
                            }
                        }

                        // 鑾峰彇鍏朵粬瀛楁
                        cJSON* currentRankPoint = cJSON_GetObjectItemCaseSensitive(squad, "currentRankPoint");
                        if (currentRankPoint != nullptr) {
                            RankList.SquadTPP.RankPoint = currentRankPoint->valueint;
                        }

                        cJSON* kda = cJSON_GetObjectItemCaseSensitive(squad, "kda");
                        if (kda != nullptr) {
                            RankList.SquadTPP.KDA = static_cast<float>(kda->valuedouble);
                        }

                        cJSON* winRatio = cJSON_GetObjectItemCaseSensitive(squad, "winRatio");
                        if (winRatio != nullptr) {
                            RankList.SquadTPP.WinRatio = static_cast<float>(winRatio->valuedouble);
                        }
                    }
                    RankList.TPP.Tier = Utils::StringToUTF8("鏈畾绾?);
                    // 澶勭悊 Solo TPP
                    cJSON* solo = cJSON_GetObjectItemCaseSensitive(rankedGameModeStats, "solo");
                    if (solo != nullptr) {
                        // 瑙ｆ瀽 "currentTier"
                        cJSON* currentTier = cJSON_GetObjectItemCaseSensitive(solo, "currentTier");
                        if (currentTier != nullptr) {
                            cJSON* tier = cJSON_GetObjectItemCaseSensitive(currentTier, "tier");
                            cJSON* subTier = cJSON_GetObjectItemCaseSensitive(currentTier, "subTier");

                            if (tier && subTier) {
                                // 鎷兼帴 "tier" 鍜?"subTier"
                                RankList.TPP.Tier = std::string(tier->valuestring) + " " + subTier->valuestring;
                            }
                        }

                        // 鑾峰彇鍏朵粬瀛楁
                        cJSON* currentRankPoint = cJSON_GetObjectItemCaseSensitive(solo, "currentRankPoint");
                        if (currentRankPoint != nullptr) {
                            RankList.TPP.RankPoint = currentRankPoint->valueint;
                        }

                        cJSON* kda = cJSON_GetObjectItemCaseSensitive(solo, "kda");
                        if (kda != nullptr) {
                            RankList.TPP.KDA = static_cast<float>(kda->valuedouble);
                        }

                        cJSON* winRatio = cJSON_GetObjectItemCaseSensitive(solo, "winRatio");
                        if (winRatio != nullptr) {
                            RankList.TPP.WinRatio = static_cast<float>(winRatio->valuedouble);
                        }
                    }

                    // 淇濆瓨鎺掑悕鏁版嵁
                    Data::SetPlayerRankListsItem(name, RankList);

                    cJSON_Delete(root);  // 閲婃斁瑙ｆ瀽鐨?JSON 鏁版嵁
                }
                else {
                    std::lock_guard<std::mutex> lock(queryMutex);
                    QueryCount[name]++;
                }
                }));
        }

        // 绛夊緟鎵€鏈夊紓姝ヤ换鍔″畬鎴?        for (auto& fut : futures_list) {
            fut.get();
        }

    }
    //std::string extractPaidInterface(const std::string& Name, const std::string& ModeName) {
    //    std::cout << "Name: " << Name << std::endl;
    //    std::string url = "/getRankByName?name=" + Name;
    //    std::string data = sendGetRequestWithWininet("pubg.000881.xyz", 0, url);
    //    return data;

    //}
    std::string extractPaidInterface(const std::string& Name, const std::string& ModeName) {
        std::cout << "Name: " << Name << std::endl;
        std::string url = "/getRankByName?name=" + Name;
        std::string data = sendGetRequestWithWininet("pubg.000881.xyz", 0, url);//1504鏌ヨ锛?5.205.30.22", 8080,
                                                                                //pubg.000881.xyz", 0,
        return data;

    }

    static void Update()
    {
        Rank rank;
        std::unordered_map<std::string, PlayerRankList> PlayerRankLists;
        std::vector<std::string> pPlayers;
        std::unordered_map<std::string, bool> QuerySuccess;
        std::unordered_map<std::string, int> QueryCount;


        while (true)
        {
            if (GameData.Scene != Scene::Gaming)
            {
                Sleep(GameData.ThreadSleep);
                continue;
            }

            if (!GameData.Config.PlayerList.RankMode)
            {
                Sleep(GameData.ThreadSleep);
                continue;
            }
            //https://pubg.op.gg/api/users/5e218a2030fa6b00140eee2/ranked-stats?season=pc-2018-31&queue_size=1&mode=competitive-tpp

            if (GameData.Config.PlayerList.RankMode == 1)
            {
                GameData.Config.ESP.RankModeUrl = "/ranked-stats?season=pc-2018-" + std::string(GameData.Config.ESP.RankSize) + "&queue_size=1&mode=competitive-tpp";
            }
            else if (GameData.Config.PlayerList.RankMode == 2)
            {
                GameData.Config.ESP.RankModeUrl = "/ranked-stats?season=pc-2018-" + std::string(GameData.Config.ESP.RankSize) + "&queue_size=4&mode=competitive-tpp";
            }
            else if (GameData.Config.PlayerList.RankMode == 3)
            {
                GameData.Config.ESP.RankModeUrl = "/ranked-stats?season=pc-2018-" + std::string(GameData.Config.ESP.RankSize) + "&queue_size=1&mode=competitive-fpp";
            }
            else if (GameData.Config.PlayerList.RankMode == 4)
            {
                GameData.Config.ESP.RankModeUrl = "/ranked-stats?season=pc-2018-" + std::string(GameData.Config.ESP.RankSize) + "&queue_size=4&mode=competitive-fpp";
            }

            PlayerRankLists = Data::GetPlayerRankLists();

            if (!GameData.Config.ESP.Tier && !GameData.Config.ESP.RankPoint && !GameData.Config.ESP.KDA)
            {
                PlayerRankLists.clear();
                pPlayers.clear();
                QuerySuccess.clear();
                QuerySuccess.clear();
                Sleep(GameData.ThreadSleep);
                continue;
            }

            for (auto& player : PlayerRankLists)
            {
                if (QuerySuccess.find(player.first) == QuerySuccess.end())
                {
                    QuerySuccess[player.first] = false;
                }

                if (QueryCount.find(player.first) == QueryCount.end())
                {
                    QueryCount[player.first] = 0;
                }

                if (QuerySuccess[player.first])
                {
                    continue;
                }

                pPlayers.push_back(player.first);
            }

            if (!pPlayers.empty())
            {
                rank.concurrentRequests(pPlayers, QuerySuccess, QueryCount);
            }


            PlayerRankLists.clear();
            pPlayers.clear();
        }
    }



    //鍘籋ack.h 鏂囦欢鍐呮妸 std::thread UpdateRankThread(Rank::Update); 鏀规垚 std::thread UpdateRankThread(Rank::UpdatePaid);
    static void UpdatePaid() {

        Rank rank;
        std::unordered_map<std::string, PlayerRankList> PlayerRankLists;
        std::vector<std::string> pPlayers;
        std::unordered_map<std::string, bool> QuerySuccess;
        std::unordered_map<std::string, int> QueryCount;
        //Utils::Log(1, "%s", Utils::StringToUTF8(rank.extractPaidInterface("SSIBHACK", "TPP")).c_str());

        while (true) {

            if (GameData.Scene != Scene::Gaming) {
                Sleep(GameData.ThreadSleep);
                continue;
            }

            if (!GameData.Config.PlayerList.RankMode) {
                Sleep(GameData.ThreadSleep);
                continue;
            }

            if (GameData.Config.PlayerList.RankMode == 1)
            {
                GameData.Config.ESP.RankModeUrl = "/ranked-stats?season=pc-2018-" + std::string(GameData.Config.ESP.RankSize) + "&queue_size=1&mode=competitive-tpp";
            }
            else if (GameData.Config.PlayerList.RankMode == 2)
            {
                GameData.Config.ESP.RankModeUrl = "/ranked-stats?season=pc-2018-" + std::string(GameData.Config.ESP.RankSize) + "&queue_size=4&mode=competitive-tpp";
            }
            else if (GameData.Config.PlayerList.RankMode == 3)
            {
                GameData.Config.ESP.RankModeUrl = "/ranked-stats?season=pc-2018-" + std::string(GameData.Config.ESP.RankSize) + "&queue_size=1&mode=competitive-fpp";
            }
            else if (GameData.Config.PlayerList.RankMode == 4)
            {
                GameData.Config.ESP.RankModeUrl = "/ranked-stats?season=pc-2018-" + std::string(GameData.Config.ESP.RankSize) + "&queue_size=4&mode=competitive-fpp";
            }

            PlayerRankLists = Data::GetPlayerRankLists();
            if (!GameData.Config.ESP.Tier && !GameData.Config.ESP.RankPoint && !GameData.Config.ESP.KDA) {
                PlayerRankLists.clear();
                pPlayers.clear();
                QuerySuccess.clear();
                QuerySuccess.clear();
                Sleep(GameData.ThreadSleep);
                continue;
            }

            for (auto& player : PlayerRankLists) {
                if (QuerySuccess.find(player.first) == QuerySuccess.end()) {
                    QuerySuccess[player.first] = false;
                }

                if (QueryCount.find(player.first) == QueryCount.end()) {
                    QueryCount[player.first] = 0;
                }

                if (QuerySuccess[player.first]) {
                    continue;
                }

                pPlayers.push_back(player.first);
            }

            if (!pPlayers.empty()) {
                rank.concurrentRequestsPaid(pPlayers, QuerySuccess, QueryCount);
            }

            PlayerRankLists.clear();
            pPlayers.clear();
        }
    }
};
