#!/usr/bin/python
import argparse
import re
from string import ascii_uppercase
import random
import sys

class GearTranslator:

    def __init__(self, seed, max_spokes=7):
        # Prepare a set of randomized A-Z gears
        self.gears = {}
        
        # 2684
        letters = list(ascii_uppercase)
        random.seed(seed)
        for spokes in range(2,max_spokes+1):
            random.shuffle(letters)
            self.gears[spokes] = list(letters)
        self.gears_in_use = list()


    """ Turns a gear by a specified number of teeth """
    def _rotate(self, spokes, teeth):
        #print "Rotating %d-spoke gear by %d teeth" % (spokes, teeth)
        gear_copy = self.gears[spokes]
        teeth = teeth % len(gear_copy)
        self.gears[spokes] = gear_copy[teeth:] + gear_copy[:teeth]
    

    """ Chooses specified gears from a larger pool,
        sets their positions with the provided key """
    def select_and_set_gears(self, gear_order, cipher):
        # Empty any previous gear sets
        self.gears_in_use = list()
        # Iterate through each selected gear
        for spokes, cipher_letter in zip(gear_order, cipher):
            # How far do we need to turn the gear to get the requested letter at the front?
            teeth = self.gears[spokes].index(cipher_letter)
            # Rotate the selected gear that far
            self._rotate(spokes, teeth)
            # Take note of the selected gears and their order
            self.gears_in_use.append(spokes)
            #self._show_gears()
    

    """ Debugging: Prints the currently selected gears, and their rotation"""
    def _show_gears(self):
        for spokes in self.gears_in_use:
            print "%d-spoke: %s" % (spokes, ''.join(self.gears[spokes]))
        print ""
        pass
    

    """ Takes a set of prepared gears, and a plaintext string, encrypts it """
    def encrypt(self, plain_text, spacing=False):
        # Clean the plaintext of non-letter characters
        plain_text = re.sub(r'[^A-Z]', '', plain_text.upper())
        # Encrypt the message
        # Move through ciphertext, a letter at a time
        cipher_text = list()
        for msg_index, plain_letter in enumerate(plain_text):
            # On encryption, input gear alternates. Find which gear to use as input
            current_idx = (msg_index % (len(self.gears_in_use) - 1)) + 1
            current_spokes = self.gears_in_use[current_idx]
            # Determine how many teeth it'll need to rotate
            rotation_teeth = self.gears[current_spokes].index(plain_letter)
            # Rotate all the gears by that amount, in alternating directions
            for idx, spokes in enumerate(self.gears_in_use):
                self._rotate(spokes, rotation_teeth * pow(-1, (idx-current_idx) % 2))
            # Now read off the encrypted letter, always from gear 0
            cipher_letter = self.gears[self.gears_in_use[0]][0]
            cipher_text.append(cipher_letter)
            #print "Encrypting '%s' -> '%s' (from %d-spoke gear)" % (plain_letter, cipher_letter, current_spokes)
            #self._show_gears()
        # Pretty it up into standard four-character block output
        cipher_text = ''.join(cipher_text)
        if spacing:
            cipher_text = [cipher_text[i:i+4] for i in range(0, len(cipher_text), 4)]
            cipher_text = ' '.join(cipher_text)
        return cipher_text
    

    """ Takes a set of prepared gears, and a ciphertext string, decrypts it """
    def decrypt(self, cipher_text, spacing=False):
        # Ciphertext should already be clean, but let's do this step anyway
        cipher_text = re.sub(r'[^A-Z]', '', cipher_text.upper())
        # Decrypt the message
        # Move through plaintext, a letter at a time
        plain_text = list()
        for msg_index, cipher_letter in enumerate(cipher_text):
            # How far do we need to turn the lead gear to get to this number?
            # On decryption, input gear is always gear 0
            rotation_teeth = self.gears[self.gears_in_use[0]].index(cipher_letter)
            # Rotate all the gears by that amount, in alternating directions
            for idx, spokes in enumerate(self.gears_in_use):
                self._rotate(spokes, rotation_teeth * pow(-1, idx % 2))
            # Now read off the decrypted letter, from alternating gears
            current_spokes = self.gears_in_use[(msg_index % (len(self.gears_in_use) - 1)) + 1]
            plain_letter = self.gears[current_spokes][0]
            plain_text.append(plain_letter)
            #print "Decrypting '%s' -> '%s' (from %d-spoke gear)" % (cipher_letter, plain_letter, current_spokes)
            #self._show_gears()
        # Pretty it up into standard four-character block output
        plain_text = ''.join(plain_text)
        if spacing:
            plain_text = [plain_text[i:i+4] for i in range(0, len(plain_text), 4)]
            plain_text = ' '.join(plain_text)
        return plain_text
    

def main():
    parser = argparse.ArgumentParser(description='Encrypt/Decrypt messages')
    parser.add_argument('--gears', '-g', required=True, nargs='+', type=int, help='Which gears, and their order')
    parser.add_argument('--key', '-k', required=True, help='The key to use when setting up the gears')
    parser.add_argument('--seed', '-s', default=2684, type=int, help='The random seed to use')
    parser.add_argument('--show', default=False, action='store_true', help='Show the gear layout used')
    group = parser.add_mutually_exclusive_group()
    group.add_argument('--encrypt', '-e', default=False, action='store_true', help='Encrypt the string provided')
    group.add_argument('--decrypt', '-d', default=False, action='store_true', help='Decrypt the string provided')
    parser.add_argument('message', nargs='*', help='The message to encrypt/decrypt')
    args = parser.parse_args()
    # We want this to be a string, not a list
    args.message = ''.join(args.message)

    if len(args.gears) != len(args.key):
        sys.stderr.write("The number of gears provided, and the length of the key must match.\n")
        sys.exit(1)

    if not args.encrypt and not args.decrypt:
        sys.stderr.write("Please specify whether to encrypt or decrypt\n")
        sys.exit(1)

    # Initialize a GearTranslator
    gt = GearTranslator(args.seed, max_spokes=max(args.gears))
    
    # Set a combination
    gt.select_and_set_gears(args.gears, args.key.upper())
    if args.show:
        gt._show_gears()
     
    if args.encrypt:
        print gt.encrypt(args.message, spacing=True)
    else:
        print gt.decrypt(args.message, spacing=True)

if __name__ == "__main__":
    main()
