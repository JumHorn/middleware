/**
 * @file json_test.cpp
 * @author JumHorn (JumHorn@gamil.com)
 * @brief json example usage
 * @version 0.1
 * @date 2020-11-24
 *
 * @copyright Copyright (c) 2020
 *
 */

#include <iostream>
#include <string>

#include "json.hpp"

using json = nlohmann::json;

/*
{
  "pi": 3.141,
  "happy": true,
  "name": "Niels",
  "nothing": null,
  "answer": {
    "everything": 42
  },
  "list": [1, 0, 2],
  "object": {
    "currency": "USD",
    "value": 42.99
  }
}
*/

int main()
{
	//this is the normal way

	// create an empty structure (null)
	json j0;

	// add a number that is stored as double (note the implicit conversion of j0 to an object)
	j0["pi"] = 3.141;

	// add a Boolean that is stored as bool
	j0["happy"] = true;

	// add a string that is stored as std::string
	j0["name"] = "Niels";

	// add another null object by passing nullptr
	j0["nothing"] = nullptr;

	// add an object inside the object
	j0["answer"]["everything"] = 42;

	// add an array that is stored as std::vector (using an initializer list)
	j0["list"] = {1, 0, 2};

	// add another object (using an initializer list of pairs)
	j0["object"] = {{"currency", "USD"}, {"value", 42.99}};

	//this is the fashion way
	json j1 = {
		{"pi", 3.141},
		{"happy", true},
		{"name", "Niels"},
		{"nothing", nullptr},
		{"answer", {{"everything", 42}}},
		{"list", {1, 0, 2}},
		{"object", {{"currency", "USD"}, {"value", 42.99}}}};

	//even string literal
	// create object from string literal
	json j2 = "{ \"happy\": true, \"pi\": 3.141 }"_json;

	// or even nicer with a raw string literal
	auto j3 = R"(
		{
			"happy": true,
			"pi": 3.141
		}
		)"_json;

	// explicit conversion to string
	std::string s = j2.dump(); // {"happy":true,"pi":3.141}
	std::cout << s << std::endl;

	// serialization with pretty printing
	// pass in the amount of spaces to indent
	std::cout << j3.dump(4) << std::endl;
	// {
	//     "happy": true,
	//     "pi": 3.141
	// }

	auto iter = j3.find("happy");
	if (iter != j3.end())
	{
		std::cout << *iter << std::endl;
	}

	//for more
	//https: //github.com/nlohmann/json

	return 0;
}