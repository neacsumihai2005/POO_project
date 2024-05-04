# Training Log App

Upon running the main.cpp, the console will output the current PRs for each recorded exercise. These PRs will be gathered from *test.txt* at the start of the program, and *test.txt* will be updated when the program stops. An user menu will pop up with on-screen instructions. This menu includes the following options:
1. start a new workout (and enther the following loop:)
    1. input an exercise's name (unique string); exit loop if input = "stop"
        1. if the exercise doesn't yet exist in the list, it will be inserted
    3. input the weight for the exercise (kg) (integer)
    4. input the reps of the exercise (an array of integers that ends with 0)
    5. jump to step (i)
2. modify records
    1. delete a certain exercise
        1. user will be prompted with on-screen instructions
    3. modify a certain PR manually
        1. user will be prompted with on-screen instructions
3. print current global PRs

The program notifies the user when he hits a new PR with a console message.


## Technical details
The fitness.h header contains the workoutDiary namespace. Inside the workoutDiary namespace we have 4 classes:
- exerciseType
- exercise
- workout
- calendaristicDate

*exerciseType* is the base template for an exercise; it's the class that holds the name, description and PRs of a certain movement.
*exercise* is characterized by an exerciseType, a weight integer and a set of reps performed with the weight.
*workout* is a collection of multiple *exercise* instances; it is also characterized by a calendaristicDate
*calendaristicDate* is a class that holds information about a certain date (day, month, year); it also has built-in functions to verify the validity of a date along with overloaded operators

