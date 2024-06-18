#!/bin/bash

# File to store account details
ACCOUNT_FILE="accounts.txt"

# Function to generate a unique account number
generate_account_number() {
    while true; do
        ACCOUNT_NUMBER=$((RANDOM % 9000000000 + 1000000000))
        if ! grep -q "$ACCOUNT_NUMBER" "$ACCOUNT_FILE"; then
            echo "$ACCOUNT_NUMBER"
            return
        fi
    done
}

# Function to create a new account
create_account() {
    ACCOUNT_NUMBER=$(generate_account_number)
    OWNER=$1
    BALANCE=0
    echo "$ACCOUNT_NUMBER,$OWNER,$BALANCE" >> "$ACCOUNT_FILE"
    echo "Account created. Account number: $ACCOUNT_NUMBER"
}

# Function to deposit money
deposit() {
    ACCOUNT_NUMBER=$1
    AMOUNT=$2
    if grep -q "$ACCOUNT_NUMBER" "$ACCOUNT_FILE"; then
        BALANCE=$(grep "$ACCOUNT_NUMBER" "$ACCOUNT_FILE" | cut -d ',' -f3)
        OWNER=$(grep "$ACCOUNT_NUMBER" "$ACCOUNT_FILE" | cut -d ',' -f2)
        NEW_BALANCE=$((BALANCE + AMOUNT))
        sed -i "s/$ACCOUNT_NUMBER,$OWNER,$BALANCE/$ACCOUNT_NUMBER,$OWNER,$NEW_BALANCE/" "$ACCOUNT_FILE"
        echo "Deposited $AMOUNT. New balance: $NEW_BALANCE"
    else
        echo "Account not found."
    fi
}

# Function to withdraw money
withdraw() {
    ACCOUNT_NUMBER=$1
    AMOUNT=$2
    if grep -q "$ACCOUNT_NUMBER" "$ACCOUNT_FILE"; then
        BALANCE=$(grep "$ACCOUNT_NUMBER" "$ACCOUNT_FILE" | cut -d ',' -f3)
        OWNER=$(grep "$ACCOUNT_NUMBER" "$ACCOUNT_FILE" | cut -d ',' -f2)
        if [ "$BALANCE" -lt "$AMOUNT" ]; then
            echo "Insufficient funds."
        else
            NEW_BALANCE=$((BALANCE - AMOUNT))
            sed -i "s/$ACCOUNT_NUMBER,$OWNER,$BALANCE/$ACCOUNT_NUMBER,$OWNER,$NEW_BALANCE/" "$ACCOUNT_FILE"
            echo "Withdrew $AMOUNT. New balance: $NEW_BALANCE"
        fi
    else
        echo "Account not found."
    fi
}

# Check if the account file exists, if not create it
if [ ! -f "$ACCOUNT_FILE" ]; then
    touch "$ACCOUNT_FILE"
fi

# Main script to manage the bank operations
while true; do
    echo "1. Create Account"
    echo "2. Deposit Money"
    echo "3. Withdraw Money"
    echo "4. Exit"
    read -p "Choose an option: " OPTION

    case $OPTION in
        1)
            read -p "Enter account owner name: " OWNER
            create_account "$OWNER"
            ;;
        2)
            read -p "Enter account number: " ACCOUNT_NUMBER
            read -p "Enter amount to deposit: " AMOUNT
            deposit "$ACCOUNT_NUMBER" "$AMOUNT"
            ;;
        3)
            read -p "Enter account number: " ACCOUNT_NUMBER
            read -p "Enter amount to withdraw: " AMOUNT
            withdraw "$ACCOUNT_NUMBER" "$AMOUNT"
            ;;
        4)
            exit 0
            ;;
        *)
            echo "Invalid option. Please try again."
            ;;
    esac
done
