#include <functional>
#include <iostream>
#include <string>
#include <vector>

namespace JS {

/**
 * @brief map / converter function - native to JS.
 *
 * @tparam Item The original template type of the std::vector
 * @tparam Product The result template type of the std::vector
 * @param values The initial std::vector
 * @param converter The function that takes in a single type Item and convert to type Product
 * @return std::vector<Product>
 */
template <typename Item, typename Product>
// map with index parameter
std::vector<Product> map(const std::vector<Item>& values, std::function<Product(const Item&, int)> converter) {
  std::vector<Product> result;
  for (int idx = 0; idx < values.size(); idx++) {
    result.push_back(converter(values[idx], idx));
  }

  return result;
}
// map with out index parameter
template <typename Item, typename Product>
std::vector<Product> map(const std::vector<Item>& values, std::function<Product(const Item&)> converter) {
  std::vector<Product> result;
  for (Item& item : values) result.push_back(converter(item));

  return result;
}

/**
 * @brief Filters a std::vector of values - native to JS
 *
 * @tparam Item The std::vector template type
 * @param values The original std::vector of values
 * @param condition The function that takes type and returns a bool
 * @return std::vector<Item>
 */
template <typename Item>
std::vector<Item> filter(const std::vector<Item>& values, std::function<bool(const Item&)> condition) {
  std::vector<Item> result;
  for (const Item& value : values) {
    if (condition(value) == true) result.push_back(value);
  }
  
  return result;
}

// Filter with index param
template <typename Item>
std::vector<Item> filter(const std::vector<Item>& values, std::function<bool(const Item&, int)> condition) {
  std::vector<Item> result;
  for (int idx = 0; idx < values.size(); idx++) {
    if (condition(values[idx], idx) == true) result.push_back(values[idx]);
  }
  
  return result;
}

/**
 * @brief Reduces a std::vector of values to a product - native to JS
 *
 * @tparam Item the std::vector template type
 * @param values The original std::vector of values
 * @param reducer The function that takes the accumulated Product with the current Item iterated, and modifies the product.
 * @return Product The result reduced from values
 */
template <typename Item, typename Product>
Product reduce(const std::vector<Item>& values, std::function<Product(Product&, const Item&)> reducer) {
  Product result;
  for (const Item& value : values) {
    result = reducer(result, value);
  }
  
  return result;
}
}  // namespace JS

template <typename Item>
void print(const std::vector<Item>& items) {
  for (const Item& item : items) std::cout << item << " ";
  std::cout << "\n";
}

class Person {
 public:
  std::string firstName;
  std::string lastName;
  int age;

  Person(std::string firstName, std::string lastName, int age) {
    this->firstName = firstName;
    this->lastName = lastName;
    this->age = age;
  }

  friend std::ostream& operator<<(std::ostream& out, const Person& person) {
    out << "Person("
        << person.firstName + ", "
        << person.lastName + ", "
        << person.age
        << ")";

    return out;
  }
};

int main() {
  // A group of friends walk into a bar
  std::vector<Person> people = {
      Person("Butter", "Riolu", 22),
      Person("Farmer", "Joes", 21),
      Person("Juke", "Duke", 21),
      Person("Life", "Happens", 24),
      Person("Looped", "Needs Help", 18),
      Person("Land", "Woof", 21)};

  print(people);

  // Filter for people over 21 years old
  std::vector<Person> canDrinkAlcohol = JS::filter<Person>(
      people, [](const Person& dude) -> bool {
        return dude.age >= 21;
      });
  print(canDrinkAlcohol);
  
  // suppose the bar bouncer only allows every other person
  std::vector<Person> everyEvenPerson = JS::filter<Person>(
      canDrinkAlcohol, [](const Person& dude, int idx) -> bool {
        return idx % 2 == 0;
      });
  print(everyEvenPerson);
  
  // get the full names, prefixed by position in queue (ex: "1. Butter Riolu")
  std::vector<std::string> names = JS::map<Person, std::string>(
      everyEvenPerson, [](const Person& dude, int idx) -> std::string {
        std::string position = std::to_string(idx + 1);
        return position + ". " + dude.firstName + " " + dude.lastName;
      });

  print(names);
  // compute average age of everyone who attempted to join
  double totalAge = JS::reduce<Person, double>(
      people, [](double totalAge, const Person& dude) -> double {
        return totalAge + dude.age;
      });

  double averageAge = totalAge / people.size();
  std::cout << "total age: " << totalAge << "\n";
  std::cout << "average age: " << averageAge << "\n";
}
