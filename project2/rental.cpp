#include <cstdlib>
#include <iomanip>
#include <iostream>

int main() {
  // On the run of a succesful path, this is set to EXIT_SUCCESS
  int errc = EXIT_FAILURE;

  // Input variables
  int odom_s, odom_e, rental_days, starting_month;
  std::string name, lux_car;

  // Read input ints, ignoring input after the integer
  std::cout << "Odometer at start: ";
  std::cin >> odom_s;
  std::cin.ignore(10000, '\n');

  std::cout << "Odometer at end: ";
  std::cin >> odom_e;
  std::cin.ignore(10000, '\n');

  std::cout << "Rental days: ";
  std::cin >> rental_days;
  std::cin.ignore(10000, '\n');

  // Read string until newline
  std::cout << "Customer name: ";
  getline(std::cin, name);

  std::cout << "Luxury car? (y/n): ";
  getline(std::cin, lux_car);

  std::cout << "Starting month (1=Jan, 2=Feb, etc.): ";
  std::cin >> starting_month;
  std::cin.ignore(10000, '\n');

  std::cout << "---" << std::endl;

  // Check error conditions
  if (odom_s < 0) {
    std::cout << "The starting odometer reading must not be negative.";
  } else if (odom_e < odom_s) {
    std::cout << "The ending odometer reading must be at least as large as the "
                 "starting reading.";
  } else if (rental_days <= 0) {
    std::cout << "The number of rental days must be positive.";
  } else if (name.empty()) {
    std::cout << "You must enter a customer name.";
  } else if (lux_car != "y" && lux_car != "n") {
    std::cout << "You must enter y or n.";
  } else if (starting_month < 1 || starting_month > 12) {
    std::cout << "The month number must be in the range 1 through 12.";
  } else {
    // To avoid floating point errors, the calculation is in cents.
    int charge = (lux_car == "y" ? 7500 : 4500) * rental_days;
    int miles = odom_e - odom_s;

    // avoiding modifying miles for reusability
    // (ex: what if the spec was changed to print miles at the end
    bool is_winter = starting_month == 12 || starting_month <= 3;
    // calculate charge from base for # of miles and the current extra milage.
    if (miles < 100) {
      charge += miles * 33;
    } else if (miles < 600) {
      charge += 3300 + (is_winter ? 33 : 27) * (miles - 100);
    } else {
      charge += 3300 + (is_winter ? 33 : 27) * 500 + (miles - 600) * 21;
    }

    // Prints out charge, setting the cents component to be left-padded to length
    // two with zeros.
    std::cout << "The rental charge for " << name << " is $" << charge / 100
              << "." << std::setfill('0') << std::setw(2) << charge % 100;
    errc = EXIT_SUCCESS;
  }
  std::cout << std::endl;

  return errc;
}
