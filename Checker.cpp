#include <iostream>
#include "json.hpp"
#include <chrono>
#include <curl/curl.h>
#include <csignal>
using namespace std;
vector<double> mediane[158];
string arr[100];
int quam;
double average[100]={0};
double answermed[100]={0};
void signalHandler(int signum)
{


    cout << "Average values "<< "Mediane values\n";
    for(int k = 1;k<quam+1;k++)
    {
        cout << arr[k] << ":" << average[k]<<"     " <<answermed[k-1]<<"\n";
    }
    exit(signum);
}
size_t writeFunction(void *ptr, size_t size, size_t nmemb, std::string* data) {
    data->append((char*) ptr, size * nmemb);
    return size * nmemb;
}
int main()
{
    CURL*curl;
    CURLcode res;


    bool flag = true;



    signal(SIGINT,signalHandler);

    int counter=1;
        while (flag) {

            string str;
            curl_global_init(CURL_GLOBAL_ALL);



            if(curl) {
                curl = curl_easy_init();
            curl_easy_setopt(curl, CURLOPT_URL, "https://www.cbr-xml-daily.ru/daily_json.js");
            curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, writeFunction);
            curl_easy_setopt(curl, CURLOPT_WRITEDATA, &str);
            auto start_time = std::chrono::steady_clock::now();
            auto end_time = std::chrono::steady_clock::now();

            while ((std::chrono::duration_cast<std::chrono::seconds>(end_time - start_time)).count() < 10)
            { end_time = std::chrono::steady_clock::now(); }
            res = curl_easy_perform(curl);
                if (res != CURLE_OK) {
                    cout << (stderr, "curl_easy_perform() returned %s\n", curl_easy_strerror(res));
                    curl_easy_cleanup(curl);
                }



            auto j = nlohmann::json::parse(str);
            // cout << j["Valute"];
                int i = 0;

            for (nlohmann::json::iterator it = j["Valute"].begin(); it != j["Valute"].end(); ++it) {
                nlohmann::json tmp = it.value();
                nlohmann::json tmp2 = it.value();
                int buff = tmp2["Nominal"];
                double buff2 = tmp["Value"];
                buff2 = buff2/buff;
                mediane[i].resize(counter+1);
                mediane[i].push_back(buff2);
                i++;
                arr[i]=it.key();
                std::cout << it.key() << " : " << buff2 << "\n";
                average[i] = ((average[i] * (counter-1)) + buff2) / counter;

                //cout << "mediane="<<mediane[counter][i-1]<< " ";
                quam = i;
            }
            for(size_t j = 0;j<i;j++)
            {
                std::sort(mediane[j].begin(), mediane[j].end());
                int b = mediane[j].size()/2;
                answermed[j]=mediane[j].back();
               // cout  << answermed[j]<<"\n";
            }
                counter++;

            string s;
            //cout << "Continue or Stop program?\n";
           // cin >> s

            curl_global_cleanup();
            curl_easy_cleanup(curl);

        }
    }



    return 0;
}
