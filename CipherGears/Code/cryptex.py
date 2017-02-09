#!/usr/bin/python

import re
from string import ascii_uppercase
import random

""" Turns a wheel by a specified number of teeth """
def rotate(wheel, n):
    #print "rotating " + str(n)
    n = n % len(wheel)
    return wheel[n:] + wheel[:n]

""" Chooses specified wheels from a larger pool,
    sets their positions with the provided key """
def select_and_set_wheels(wheels, wheel_order, key):
    wheels_in_use = list()
    # Local copy of the wheels we'll be rotating
    for wheel_number, key_letter in zip(wheel_order, key):
        # How far do we need to turn the wheel to get the key at the front?
        index_of_key = wheels[wheel_number].index(key_letter)
        # Rotate the selected wheel that far
        rotated_wheel = rotate(wheels[wheel_number], index_of_key)
        # Add a copy to our list of wheels in play
        wheels_in_use.append(rotated_wheel)
    return wheels_in_use

""" Debugging: prints the wheels in the passed set """
def show_wheels(wheel_set):
    for wheel in wheel_set:
        print ''.join(wheel)
    pass

""" Takes a set of prepared wheels, and a plaintext string, encrypts it """
def encrypt(wheel_set, plain_text):
    # Clean the plaintext of non-letter characters
    plain_text = re.sub(r'[^A-Z]', '', plain_text.upper())
    # Encrypt the message
    # Move through ciphertext, a letter at a time
    cipher_text = list()
    for msg_index, plain_letter in enumerate(plain_text):
        # How far do we need to turn the lead wheel to get to this number?
        current_wheel = (msg_index % (len(wheel_set) - 1)) + 1
        rotation_distance = wheel_set[current_wheel].index(plain_letter)
        # Rotate all the wheels by that amount, in alternating directions
        for wheel_index, wheel in enumerate(wheel_set):
            wheel_set[wheel_index] = rotate(wheel, rotation_distance * pow(-1, (wheel_index-current_wheel) % 2))
        # Now read off the encrypted letter, from alternating wheels
        cipher_letter = wheel_set[0][0]
        print "Encrypting '" + plain_letter + "' -> '" + cipher_letter + "' (from wheel " + str(current_wheel+1) + ")"
        #show_wheels(wheel_set)
        cipher_text.append(cipher_letter)
    # Pretty it up into standard four-character block output
    cipher_text = ''.join(cipher_text)
    cipher_text = [cipher_text[i:i+4] for i in range(0, len(cipher_text), 4)]
    return ' '.join(cipher_text)

""" Takes a set of prepared wheels, and a ciphertext string, decrypts it """
def decrypt(wheel_set, cipher_text):
    # Ciphertext should already be clean, but let's do this step anyway
    cipher_text = re.sub(r'[^A-Z]', '', cipher_text.upper())
    # Decrypt the message
    # Move through plaintext, a letter at a time
    plain_text = list()
    for msg_index, cipher_letter in enumerate(cipher_text):
        # How far do we need to turn the lead wheel to get to this number?
        rotation_distance = wheel_set[0].index(cipher_letter)
        # Rotate all the wheels by that amount, in alternating directions
        for wheel_index, wheel in enumerate(wheel_set):
            wheel_set[wheel_index] = rotate(wheel, rotation_distance * pow(-1, wheel_index % 2))
        # Now read off the decrypted letter, from alternating wheels
        current_wheel = (msg_index % (len(wheel_set) - 1)) + 1
        plain_letter = wheel_set[current_wheel][0]
        print "Decrypting '" + cipher_letter + "' -> '" + plain_letter + "' (from wheel " + str(current_wheel+1) + ")"
        #show_wheels(wheel_set)
        plain_text.append(plain_letter)
    # Pretty it up into standard four-character block output
    plain_text = ''.join(plain_text)
    plain_text = [plain_text[i:i+4] for i in range(0, len(plain_text), 4)]
    return ' '.join(plain_text)
    
# Prepare a set of randomized A-Z wheels
letters = list(ascii_uppercase)
wheels = {}

random.seed(2684)
for spokes in range(2,8):
    random.shuffle(letters)
    wheels[spokes] = list(letters)

for spokes in wheels:
    print str(spokes) + "-spoke Wheel: " + ''.join(wheels[spokes])
print "\n\n"

# Set a group of wheels, and a key
wheels_to_use = select_and_set_wheels(wheels, [2,4,6,7], "DOGS")
plain_text = "Attack at dawn!"
print plain_text

#show_wheels(wheels_to_use)
cipher_text = encrypt(wheels_to_use, plain_text)
print cipher_text

#show_wheels(wheels_to_use)
plain_text = decrypt(wheels_to_use, cipher_text)
print plain_text

