# Training Log App

Upon running the main.cpp, the user will be prompted with a message of the day that changes depending on the current system time and showcases the concept of Upcasting. There is also a login screen. If the username is not found in the current users list, a new user will be created. After login, the console will output the current PRs for each recorded exercise. These PRs will be gathered from *exercises.txt* at the start of the program, for each user, even if it is not the one who logged in (this helps keep the *exercises.txt* file intact after updating), and *exercises.txt* will be updated when the program stops. An user menu will pop up with on-screen instructions. This menu includes the following options:
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
4. challenge another user to either an exercise or a coin flip
5. randomly change the console color
6. revert the color change
7. inspect the price of some of the clothes for sale at the gym

The program notifies the user when he hits a new PR with a console message.


## To-do list
- port app to iOS for my own personal use
- create a GUI
- replace the .txt file with a database for storing information
- export graphs about each exercise's progression in time
- register/log-in module for multiple users
- launch to app store?
