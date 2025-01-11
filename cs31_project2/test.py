#!/usr/bin/env python3
import subprocess

# Test cases: input tuple and expected output
test_cases = [
    ((2417, 2754, 4, "Al", 'n', 10), "The rental charge for Al is $276.99"),
    ((1885, 1973, 1, "Bob", 'y', 2), "The rental charge for Bob is $104.04"),
    ((3000, 3600, 5, "Carl", 'n', 6), "The rental charge for Carl is $393.00"),
    ((4000, 4700, 7, "Dennis", 'y', 12), "The rental charge for Dennis is $744.00"),
    ((-100, 200, 3, "Edgar", 'n', 7), "The starting odometer reading must not be negative."),
    ((500, 400, 2, "Fred", 'n', 4), "The ending odometer reading must be at least as large as the starting reading."),
    ((1000, 1500, 0, "Gustav", 'n', 8), "The number of rental days must be positive."),
    ((1500, 1800, 3, "", 'n', 3), "You must enter a customer name."),
    ((2000, 2500, 2, "Horatio", 'x', 5), "You must enter y or n."),
    ((2500, 2800, 3, "Ito", 'n', 13), "The month number must be in the range 1 through 12.")
]

# Function to run test cases on the compiled binary
def run_test_cases():
    binary_path = "./rental"  # Path to the compiled binary

    for i, (inputs, expected_output) in enumerate(test_cases):
        # Prepare the input as a string
        input_data = "\n".join(map(str, inputs)) + "\n"

        # Run the binary with the input
        result = subprocess.run(binary_path, input=input_data, text=True, capture_output=True)

        # Get the output (only the last line matters)
        output_lines = result.stdout.strip().splitlines()
        actual_output = output_lines[-1] if output_lines else ""

        # Compare actual output with expected output
        if actual_output == expected_output:
            print(f"Test case {i + 1} passed.")
        else:
            print(f"Test case {i + 1} failed.")
            print(f"  Expected: {expected_output}")
            print(f"  Got: {"\n".join(output_lines)}")


# Run all test cases
if __name__ == "__main__":
    run_test_cases()
