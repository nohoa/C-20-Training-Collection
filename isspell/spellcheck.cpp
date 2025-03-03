#include "spellcheck.h"

#include <algorithm>
#include <cctype>
#include <iostream>
#include <iterator>
#include <numeric>
#include <ranges>
#include <set>
#include <vector>
#include <utility>

namespace rv = std::ranges::views;

template <typename Iterator, typename UnaryPred>
std::vector<Iterator> find_all(Iterator begin, Iterator end, UnaryPred pred);

Corpus tokenize(const std::string& source) {

  /* TODO: Implement this method */
  auto space_list = find_all(source.begin(), source.end(),isspace);

   auto generate_token= [ &source]( auto it1,  auto it2){
      //std :: cout << *it1 << std :: endl; 
       return Token{source,it1,it2};
   } ;


  Corpus tokens ;
 
   auto resulting_token = std::inserter(tokens, tokens.end()) ;
  
   auto result_token = std::transform(space_list.begin(), space_list.end()-1,space_list.begin()+1,resulting_token,generate_token);
  

  auto empty_check = [](auto curr){
    return curr.content.empty();
  } ;

  auto clean_token =  std::erase_if(tokens,empty_check);

  
  return tokens;
}
 

std::set<Mispelling> spellcheck(const Corpus& source, const Dictionary& dictionary) {
  /* TODO: Implement this method */

  auto view = source 
  | rv::filter (
    [&dictionary](Token current_word){
      return !dictionary.contains(current_word.content) ;
  });


  auto suggestion_mispell = view |
         rv::transform([&dictionary](auto curr){
       auto v_inside =  rv::filter(dictionary,[curr](const std :: string &dict_value){
             return levenshtein(dict_value,curr.content) == 1;
       }) ;
       std::set<std::string> suggestions(v_inside.begin(), v_inside.end());
       return Mispelling{curr,suggestions};
  }) ;

  auto clear_suggestion = suggestion_mispell | rv::filter([](auto set_spell){
    return set_spell.suggestions.size() != 0 ;
  });
 
 std::set<Mispelling>  ans(clear_suggestion.begin(),clear_suggestion.end());


  return ans ;
};

/* Helper methods */


#include "utils.cpp"