/* Drew Schlesener
 * Denton Jarvis
 * Kevin Cunningham
 *
 * 
*/
#include <iostream>
#include <string>
#include <vector>
#include <unordered_map>
#include <cstdio>
#include <fstream>
#include <boost/process.hpp> // -lboost_system -lpthread
using namespace std;

namespace bp = boost::process;

#include "rapidjson/document.h"
#include "rapidjson/filereadstream.h"

struct PartitionInfo {
    int idle_nodes = 0;
    std::vector<std::string> node_names;
};

int main(int argc, char** argv) {
    char buffer[20];
    char buffer2[100];
    snprintf(buffer, sizeof(buffer), "sinfo_%s.json", argv[1]);
 
    snprintf(buffer2, sizeof(buffer2), "sinfo --json > %s", buffer);
    system(buffer2);

    FILE* fp = fopen(buffer, "r");
    if (!fp) {
        std::cerr << "Failed to open sinfo.json\n";
        return 1;
    }

    // Read the JSON using a RapidJSON streaming buffer
    char readBuffer[65536];
    rapidjson::FileReadStream is(fp, readBuffer, sizeof(readBuffer));

    rapidjson::Document doc;
    doc.ParseStream(is);
    fclose(fp);
    char buffer3[20];
    snprintf(buffer3, sizeof(buffer3), "rm %s", buffer);
    system(buffer3);


    if (!doc.IsObject() || !doc.HasMember("sinfo") || !doc["sinfo"].IsArray()) {
        std::cerr << "Invalid sinfo JSON structure\n";
        return 1;
    }

    // Map partition name to its info
    std::unordered_map<std::string, PartitionInfo> partitions;

    for (const auto& entry : doc["sinfo"].GetArray()) {

        // partitions live under entry["partition"]["name"]
        if (!entry.HasMember("partition")) continue;
        const auto& partObj = entry["partition"];
        if (!partObj.HasMember("name") || !partObj["name"].IsString()) continue;

        std::string pname = partObj["name"].GetString();

        // each entry also has a "nodes" block with idle count + node names
        if (!entry.HasMember("nodes")) continue;
        const auto& nodeBlock = entry["nodes"];

        PartitionInfo& info = partitions[pname];

        // idle node count
        if (nodeBlock.HasMember("idle") && nodeBlock["idle"].IsInt()) {
            info.idle_nodes += nodeBlock["idle"].GetInt();  // accumulate
        }

        // list of nodes by name
        if (nodeBlock.HasMember("nodes") && nodeBlock["nodes"].IsArray()) {
            for (const auto& n : nodeBlock["nodes"].GetArray()) {
                if (n.IsString()) {
                    info.node_names.push_back(n.GetString());
                }
            }
        }
    }

    ofstream myfile("node_names.txt");

    // Print results
    for (const auto& [partName, info] : partitions) {
        std::cout << "Partition: " << partName << "\n";
        std::cout << "  Idle nodes: " << info.idle_nodes << "\n";
        //write node names to node_names.txt
        for (const auto& n : info.node_names) {
            myfile << "    " << n << "\n";
        }
        std::cout << "\n";
    }

    myfile.close();

    return 0;
}
