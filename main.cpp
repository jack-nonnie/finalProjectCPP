#include <iostream>
#include <vector>
#include <unordered_map>
#include <algorithm>

using namespace std;

//Class for cards
class Card
{
    friend ostream &operator<<(ostream &os, const Card &c);

private:
    int value;
    string name;

public:
    Card() {}
    Card(int v, string n)
    {
        value = v;
        name = n;
    }
    //Getters for the card class
    string getName()
    {
        return name;
    }
    int getValue()
    {
        return value;
    }
    //Used to change the value of an ace from 11 to 1 when needed
    void changeAceValue()
    {
        if (name == "ace")
        {
            value = 1;
        }
    }
    //Card destructor
    ~Card()
    {
        value = 0;
        name = "";
    }
};
//Overloading the << operator for class card
ostream &operator<<(ostream &os, const Card &c)
{
    os << c.name;
    return os;
}
//Class to model a deck of cards
class Deck
{
private:
    vector<Card> deck;
    unordered_map<string, int> value;
    vector<string> cardsNames;

public:
    int getSizeOfDeck()
    {
        return deck.size();
    }
    //Sets up a vector for the names of all the cards possible in a deck
    void setUpCardsNames()
    {
        cardsNames.push_back("two");
        cardsNames.push_back("three");
        cardsNames.push_back("four");
        cardsNames.push_back("five");
        cardsNames.push_back("six");
        cardsNames.push_back("seven");
        cardsNames.push_back("eight");
        cardsNames.push_back("nine");
        cardsNames.push_back("ten");
        cardsNames.push_back("jack");
        cardsNames.push_back("queen");
        cardsNames.push_back("king");
        cardsNames.push_back("ace");
    }
    //Creating the map from card names to their associated values
    void setUpValue()
    {
        value["two"] = 2;
        value["three"] = 3;
        value["four"] = 4;
        value["five"] = 5;
        value["six"] = 6;
        value["seven"] = 7;
        value["eight"] = 8;
        value["nine"] = 9;
        value["ten"] = 10;
        value["jack"] = 10;
        value["queen"] = 10;
        value["king"] = 10;
        value["ace"] = 11;
    }
    //Adds one deck to the private list of cards in the class. A deck of card includes 4 suits of all the types of cards.
    void addDeck()
    {
        for (int i = 0; i < cardsNames.size(); i++)
        {
            for (int j = 0; j < 4; j++)
            {
                Card c(value[cardsNames[i]], cardsNames[i]);
                deck.push_back(c);
            }
        }
    }
    //Removing and returning one card from the deck
    Card draw()
    {
        Card temp = deck[deck.size() - 1];
        deck.pop_back();
        return temp;
    }
    //Shuffling the deck of cards
    void shuffle()
    {
        std::random_shuffle(deck.begin(), deck.end());
    }
    //Default constructor
    Deck()
    {
        setUpCardsNames();
        setUpValue();
        //Standard number of decks in vegas is 7 when playing blackjack
        for (int i = 0; i < 7; i++)
        {
            addDeck();
        }
    }
    //Constructor where the number of decks of cards is picked
    Deck(int numberOfDecks)
    {
        setUpCardsNames();
        setUpValue();
        for (int i = 0; i < numberOfDecks; i++)
        {
            addDeck();
        }
    }
    //Desk deconstructor
    ~Deck()
    {
        deck.clear();
        value.clear();
        cardsNames.clear();
    }
};
//Class to model a person/dealers hand
class Hand
{
    friend ostream &operator<<(ostream &os, const Hand &h);

private:
    //contains ace and soft are different but necessary booleans to reduce computations
    //contains ace is true when an ace is in the hand while soft is when an ace is in the hand and has a value of 11 instead of 1. Soft is a blackjack terminology. Both are necessary inorder for blackjacks calculations to work.
    bool containsAce, soft, bust;
    vector<Card> hand;
    int value;
    Card hiddenCard;

public:
    Hand()
    {
        containsAce = false;
        soft = false;
        bust = false;
        value = 0;
    }
    //Adding a card to the hand.
    void addCard(Card c)
    {
        //Checking if the card is an ace for different value logic
        if (c.getName() == "ace")
        {
            if (containsAce)
            {
                c.changeAceValue();
            }
            else if (value > 10)
            {
                c.changeAceValue();
                containsAce = true;
            }
            else
            {
                containsAce = true;
                soft = true;
            }
        }
        //Adding the value of the card to the hand
        value += c.getValue();
        hand.push_back(c);
        //Checking if there is a bust
        if (value > 21)
        {
            if (soft)
            {
                soft = false;
                for (int i = 0; i < hand.size(); i++)
                {
                    hand[i].changeAceValue();
                }
                value -= 10;
            }
            else
            {
                bust = true;
            }
        }
    }
    //Adding dealers face down card
    void addHiddenCard(Card h)
    {
        hiddenCard = h;
    }
    //flipping over the dealers face down card
    void flipHiddenCard()
    {
        addCard(hiddenCard);
    }
    //checking dealer for blackjack
    bool checkHiddenBlackJack()
    {
        if (value + hiddenCard.getValue() == 21)
        {
            flipHiddenCard();
            return true;
        }
        return false;
    }
    //getters
    bool getBust()
    {
        return bust;
    }
    bool getSoft()
    {
        return soft;
    }
    int getValue()
    {
        return value;
    }
    int getSize()
    {
        return hand.size();
    }
    //Deconstructor for hand
    ~Hand()
    {
        hand.clear();
        value = 0;
        containsAce = false;
        soft = false;
        bust = false;
    }
};
//Overloading the << operator for class hand
ostream &operator<<(ostream &os, const Hand &h)
{
    for (int i = 0; i < h.hand.size(); i++)
    {
        os << h.hand[i] << " ";
    }
    os << endl;
    os << "With a value of: " << h.value << endl;
    return os;
}
//Creating a class to model a game
class Game
{
private:
    Deck deck;
    Hand dealer, player;
    int multiplier;
    bool doubleAble, bj;

public:
    Game()
    {
        multiplier = 1;
        doubleAble = false;
        bj = false;
    }
    Game(Deck d)
    {
        deck = d;
        multiplier = 1;
        doubleAble = false;
        bj = false;
    }
    Game(Deck d, Hand deal, Hand play)
    {
        deck = d;
        dealer = deal;
        player = play;
        multiplier = 1;
        doubleAble = false;
        bj = false;
    }
    //Printing the dealers hand and the players hand
    void printHand()
    {
        std::cout << "The dealer has the following hand: " << endl
                  << dealer;
        std::cout << "You has the following hand: " << endl
                  << player;
    }
    //Starting the game by dealing the dealer two cards facedown and the player two cards face up
    void start()
    {
        dealer.addCard(deck.draw());
        player.addCard(deck.draw());
        dealer.addHiddenCard(deck.draw());
        player.addCard(deck.draw());
        printHand();
    }
    //Simulating the players turn
    void playerTurn()
    {
        //checking for blackjack
        bool playing = true;
        if (player.getValue() == 21)
        {
            playing = false;
            bj = true;
            std::cout << "You got BLACK JACK! Congratulations!" << endl;
        }
        else if (dealer.checkHiddenBlackJack())
        {
            playing = false;
            bj = true;
            std::cout << "Sorry dealer got Black Jack! You Lose" << endl;
            printHand();
        }
        //Giving the option for the player to hit or stand while checking for bad inputs
        while (playing)
        {
            if (player.getSize() == 2 && doubleAble)
            {
                std::cout << "Would you like to hit, stand, or double (hit/stand/double)" << endl;
            }
            else
            {
                std::cout << "Would you like to hit or stand (hit/stand)" << endl;
            }
            string input;
            std::cin >> input;
            if (input == "double" && player.getSize() == 2 && doubleAble)
            {
                multiplier = 2;
                input = "hit";
            }
            if (input == "hit")
            {
                player.addCard(deck.draw());
                if (player.getBust())
                {
                    std::cout << "You have the following hand: " << endl
                              << player;
                    playing = false;
                }
                else
                {
                    printHand();
                }
            }
            else if (input == "stand")
            {
                playing = false;
            }
            else
            {
                std::cout << "Input was not yes or no please use the correct input";
            }
        }
    }
    //Simulating the dealers turn
    void dealerTurn()
    {
        //Only simulating the dealers hand if the player does not bust
        if (!player.getBust() && !bj)
        {
            dealer.flipHiddenCard();
            printHand();
            while (dealer.getValue() < 17)
            {
                std::cout << "Dealer is below 17 so dealer hit: " << endl;
                dealer.addCard(deck.draw());
                printHand();
            }
        }
    }
    //Checking to see who won the hand
    int winner()
    {
        if (bj)
        {
            if (player.getValue() == 21)
            {
                return 2;
            }
            else
            {
                return -1;
            }
        }
        else if (player.getBust())
        {
            std::cout << "You busted! Dealer wins!" << endl;
            return -1 * multiplier;
        }
        else if (dealer.getBust())
        {
            std::cout << "The dealer busted! You win!" << endl;
            return 1 * multiplier;
        }
        else if (dealer.getValue() > player.getValue())
        {
            std::cout << "The dealer has a " << dealer.getValue() << ". You have a " << player.getValue() << ". Dealer Wins!" << endl;
            return -1 * multiplier;
        }
        else if (player.getValue() > dealer.getValue())
        {
            std::cout << "The dealer has a " << dealer.getValue() << ". You have a " << player.getValue() << ". You Wins!" << endl;
            return 1 * multiplier;
        }
        else
        {
            std::cout << "The dealer has a " << dealer.getValue() << ". You have a " << player.getValue() << ". It is a draw" << endl;
            return 0;
        }
    }
    //Simulating the entire game. While returning the outcome
    int playAGame()
    {
        start();
        playerTurn();
        dealerTurn();
        return winner();
    }
    //getting the deck
    Deck getDeck()
    {
        return deck;
    }
    //sets wether or not a double is possible.
    void setDoubleAble(bool d)
    {
        doubleAble = d;
    }
    //deconstructor
    ~Game()
    {
        std::cout << "Game is over" << endl;
    }
};
class BlackJack
{
private:
    int balance;
    int bet;
    Deck deck;
    int wins;
    int losses;
    int draws;
    int netGain;
    int numberOfDecks;

public:
    //constructing a game with the default number of decks of 7
    BlackJack()
    {
        balance = 0;
        numberOfDecks = 7;
        Deck a(numberOfDecks);
        deck = a;
        wins = 0;
        losses = 0;
        draws = 0;
        netGain = 0;
        deck.shuffle();
    }
    //constructing a game with a user choice for the number of decks
    BlackJack(int numberOfDeck)
    {
        balance = 0;
        numberOfDecks = numberOfDeck;
        Deck a(numberOfDecks);
        deck = a;
        wins = 0;
        losses = 0;
        draws = 0;
        netGain = 0;
        deck.shuffle();
    }
    //returning the balance
    int getBalance()
    {
        return balance;
    }
    //Withdrawling an amount from the balance while checking for error conditions
    void withdrawl()
    {
        int withdrawl;
        string wi;
        std::cout << "Enter an int amount to withdrawl" << endl;
        std::cin >> wi;
        try
        {
            withdrawl = stoi(wi);
        }
        catch (exception &er)
        {
            std::cout << "Input must be an int. Converting to -1" << endl;
            withdrawl = -1;
        }
        if (withdrawl <= balance)
        {
            balance -= withdrawl;
        }
        else if (withdrawl <= 0)
        {
            std::cout << "Not valid withdrawl amount. Withdrawl amount has to be greater than 0" << endl;
        }
        else
        {
            std::cout << "Not valid withdrawl amount. Withdrawl amount has to be less than or equal to balance. Current balance is: " << balance << endl;
        }
    }
    //depositing an amount to balance while checking error conditions
    void deposit()
    {
        int deposit;
        string de;
        std::cout << "Enter an int amount to deposit" << endl;
        std::cin >> de;
        try
        {
            deposit = stoi(de);
        }
        catch (exception &err)
        {
            std::cout << "Input must be an int. Converting to -1" << endl;
            deposit = -1;
        }
        if (deposit > 0)
        {
            balance += deposit;
            std::cout << "Deposit was a success current balance is $" << balance << endl;
        }
        else
        {
            std::cout << "Not valid deposit. Deposit amount has to be greater than 0" << endl;
        }
    }
    //Simulating placing a bet while checking for error conditions
    void placeABet()
    {
        string be;
        int b;
        bool badBet = true;
        while (badBet)
        {
            std::cout << "Enter an amount to bet" << endl;
            std::cin >> be;
            try
            {
                b = stoi(be);
            }
            catch (exception &err)
            {
                std::cout << "Input must be an int. Converting to -1" << endl;
                b = -1;
            }
            if (b > balance)
            {
                std::cout << "Not a valid bet. Bet has to be less than or equal to current balance. Current balance: $" << balance << endl;
            }
            else if (b < 0)
            {
                std::cout << "Not a valid bet. Bet has to be greater than 0 " << endl;
            }
            else
            {
                bet = b;
                badBet = false;
            }
        }
    }
    //Reshuffling the deck when it gets below a user defined size
    void checkDeckSize(int reshuffleSize)
    {
        if (deck.getSizeOfDeck() < reshuffleSize)
        {
            std::cout << "Deck is too low time to reshuffle!" << endl;
            Deck a(numberOfDecks);
            deck = a;
            deck.shuffle();
        }
    }
    //Simulating playing a game of blackjack
    void playAGame()
    {
        placeABet();
        checkDeckSize(250);
        Game g(deck);
        g.setDoubleAble(balance / 2 >= bet);
        int outcome = g.playAGame();
        balance += bet * outcome;
        netGain += bet * outcome;
        if (outcome == 0)
        {
            draws++;
        }
        if (outcome == 1)
        {
            wins++;
        }
        if (outcome == -1)
        {
            losses++;
        }
        deck = g.getDeck();
    }
    //Playing blackjack
    void playBlackJack()
    {
        int choice;
        string ch;
        while (true)
        {
            std::cout << "Your current balance is: $" << balance << ". To play a game type in 1. To withdrawl an amount type in 2. To deposit an amount type in 3. To stop playing blackjack type in 9." << endl;
            std::cin >> ch;
            try
            {
                choice = stoi(ch);
            }
            catch (exception &err)
            {
                std::cout << "Input must be an int. Converting to -1" << endl;
                choice = -1;
            }

            if (choice == 1)
            {
                playAGame();
            }
            else if (choice == 2)
            {
                withdrawl();
            }
            else if (choice == 3)
            {
                deposit();
            }
            //Displaying stats of the user's history while playing
            else if (choice == 9)
            {
                std::cout << "Thank you for playing blackjack! I hope you had fun!" << endl;
                std::cout << "So far while playing blackjack your record is " << wins << "-" << draws << "-" << losses << endl;
                std::cout << "And you have a net winnings of " << netGain << endl;
                break;
            }
            //handling error inputs
            else
            {
                std::cout << "Invalid input. Please enter a valid input." << endl;
            }
        }
    }
    //Black Jack deconstructor
    ~BlackJack()
    {
        wins = 0;
        losses = 0;
        draws = 0;
        netGain = 0;
        balance = 0;
        bet = 0;
    }
};
int main()
{
    BlackJack blackjack(7);
    blackjack.playBlackJack();
    return 0;
}