
#include <iostream>
#include <string>
#include <vector>
#include <algorithm>

using namespace std;


string extend(string data) {
    string copy_data(data);
    reverse(copy_data.begin(), copy_data.end());
    for (size_t i=0; i<data.size(); ++i)
        copy_data[i] = (copy_data[i] == '1' ? '0' : '1');
    return data + '0' + copy_data;
}

string get_checksum(string data) {
    string checksum;
    for (size_t pos=0; pos<data.size(); pos+=2) {
        if (data[pos] == data[pos+1]) {
            checksum += '1';
        } else {
            checksum += '0';
        }
    }
    return checksum;
}

int main()
{
    bool first_half = false;
    string input = "10001001100000001";
    string ans;

    size_t length = (first_half ? 272 : 35651584);

    string data = input;
    while (data.size() < length)
        data = extend(data);

    data = data.substr(0, length);

    string checksum = get_checksum(data);
    while (checksum.size() % 2 == 0)
        checksum = get_checksum(checksum);

    ans = checksum;

    cout << "Answer : " << ans << endl;
    return 0;
}
