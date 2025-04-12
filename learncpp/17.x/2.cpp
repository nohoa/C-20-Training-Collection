#include<iostream>
#include<array>

int main(){
    using std :: cout ;
    constexpr std::array<char,5> alphabet{'h','e','l','l','o'};
    cout << "The length is " << alphabet.size() << std::endl;
    cout << alphabet[1] <<alphabet.at(1) << std::get<1>(alphabet);
}