import re

def validate_credit_card(card_number):
    pattern = r'^\d{4}-\d{4}-\d{4}-\d{4}$'
    if re.match(pattern, card_number):
        return True
    else:
        return False

def validate_pan(pan_number):
    pattern = r'^[A-Z]{5}\d{4}[A-Z]$'
    if re.match(pattern, pan_number):
        return True
    else:
        return False

def validate_password(password):
    pattern = r'^[A-Za-z]{1}[A-Za-z]{3,4}[@$#%!._]{1}\d{2,3}$'
    if re.match(pattern, password):
        return True
    else:
        return False

def main():
    credit_card = input("Enter credit card number: ")
    if validate_credit_card(credit_card):
        print("Valid credit card number")
    else:
        print("Invalid credit card number")

    pan = input("Enter PAN number: ")
    if validate_pan(pan):
        print("Valid PAN number")
    else:
        print("Invalid PAN number")

    password = input("Enter password: ")
    if validate_password(password):
        print("Valid password")
    else:
        print("Invalid password")

if __name__ == "__main__":
    main()
