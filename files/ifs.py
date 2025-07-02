class IfsClass:
    def check_number(self, num):
        # if/elif/else example
        if num > 0:
            print("Positive number")
        elif num == 0:
            print("Zero")
        else:
            print("Negative number")

    def loop_examples(self):
        # while loop example
        count = 0
        while count < 3:
            print(f"While loop count: {count}")
            count += 1
        else:
            print("While loop ended")

        # for loop example
        for i in range(3):
