#pragma once

#include <string>
#include <vector>

using namespace std;


bool is_digit(char ch)
{
    int shift = int(ch) - int('0');
    return (0 <= shift) && (shift <= 9);
}

// Преобразование строки целых чисел в вектор целых чисел
vector<int> line_to_vec(string line)
{
    vector<int> res;

    int n;
    string buff = "";
    for (char ch : line) {
        if (is_digit(ch))
            buff += ch;
        else
            if (buff.size() > 0) {
                n = stoi(buff);
                res.push_back(n);
                buff.clear();
            }
    }
    if (buff.size() > 0) {
        n = stoi(buff);
        res.push_back(n);
    }

    return res;
}
