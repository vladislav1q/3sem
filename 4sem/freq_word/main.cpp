#include <iostream>
#include <vector>
#include <map>
#include <algorithm>

using namespace std;

bool cmp(const pair< string, int > &string1, const pair< string, int > &string2)
{
    return string1.second > string2.second;
}

string clear_word(string str){
    char symbols[] = {'.', ',', ';', ':', '(', ')', '\"', '\'', '!', '?'};

    for(char i :symbols){
        for(auto k = str.begin(); k < str.end(); k++){
            if(*k == i){
                str.erase(k);
            }
        }
    }
    return str;
}

int main(){
    map <string, int> word;
    string buffer;
    int maxNumberWords = 30;

    cin >> buffer;
    while(buffer != "qqq"){
        transform(buffer.begin(), buffer.end(), buffer.begin(), ::tolower);
        if(word.count(buffer) == 0)
            word.insert(pair<string,int> (clear_word(buffer), 1));
        else
            word[buffer]++;
        cin >> buffer;
    }

    vector<pair< string, int > > vec(word.begin(), word.end());
    sort(vec.begin(), vec.end(), cmp);

    cout << endl;
    int k = 0;
    for (auto i : vec){
        std::cout << i.first << ' ' << i.second << std::endl;
        k++;
        if(k > maxNumberWords)
            break;
    }

    return 0;
}
