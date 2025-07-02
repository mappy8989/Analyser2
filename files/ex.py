class ExClass:
    def classify_temperature(self, temp_celsius):
        # if/elif/else example
        if temp_celsius < 0:
            return "Freezing"
        elif 0 <= temp_celsius < 15:
            return "Cold"
        elif 15 <= temp_celsius < 25:
            return "Mild"
        elif 25 <= temp_celsius < 35:
            return "Warm"
        else:
            return "Hot"

    def countdown(self, start):
        # while loop example
        print("Countdown:")
        while start > 0:
            print(start)
            start -= 1
        else:
            print("Liftoff!")

    def print_squares(self, numbers):
        # for loop example
        print("Squares of numbers:")
        for num in numbers:
            print(f"{num}^2 = {num ** 2}")

    def divide_numbers(self, numerator, denominator):
        # try/except/finally example
        try:
            result = numerator / denominator
        except ZeroDivisionError:
            print("Cannot divide by zero!")
            result = None
        finally:
            print("Division attempt finished.")
        return result

    def describe_day(self, day):
        # match/case example (Python 3.10+)
        match day.lower():
            case "monday":
                return "Start of the work week."
            case "friday":
                return "Almost weekend!"
            case "saturday" | "sunday":
                return "Weekend!"
