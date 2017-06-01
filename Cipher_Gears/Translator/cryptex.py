#!/usr/bin/python
import re
from string import ascii_uppercase
import random

class GearTranslator:

    def __init__(self, seed):
        # Prepare a set of randomized A-Z wheels
        self.gears = {}
        
        # 2684
        letters = list(ascii_uppercase)
        random.seed(seed)
        for spokes in range(2,8):
            random.shuffle(letters)
            self.gears[spokes] = list(letters)
        self.gears_in_use = list()


    """ Turns a gear by a specified number of teeth """
    def _rotate(self, spokes, teeth):
        #print "Rotating %d-spoke wheel by %d teeth" % (wheel, teeth)
        gear_copy = self.gears[spokes]
        teeth = teeth % len(gear_copy)
        self.gears[spokes] = gear_copy[teeth:] + gear_copy[:teeth]
    

    """ Chooses specified wheels from a larger pool,
        sets their positions with the provided key """
    def select_and_set_wheels(self, gear_order, cipher):
        # Empty any previous gear sets
        self.gears_in_use = list()
        # Local copy of the wheels we'll be rotating
        for spokes, cipher_letter in zip(gear_order, cipher):
            # How far do we need to turn the gear to get the requested letter at the front?
            teeth = self.gears[spokes].index(cipher_letter)
            # Rotate the selected gear that far
            self._rotate(spokes, teeth)
            # Take note of the selected gears and their order
            self.gears_in_use.append(spokes)
    

    """ Debugging: Prints the currently selected gears, and their rotation"""
    def _show_wheels(self):
        for spokes in self.gears_in_use:
            print "%d-spoke: %s" % (spokes, ''.join(self.gears[spokes]))
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
            # Rotate all the wheels by that amount, in alternating directions
            for idx, spokes in enumerate(self.gears_in_use):
                self._rotate(spokes, rotation_teeth * pow(-1, (idx-current_idx) % 2))
            # Now read off the encrypted letter, always from gear 0
            cipher_letter = self.gears[self.gears_in_use[0]][0]
            cipher_text.append(cipher_letter)
            #print "Encrypting '%s' -> '%s' (from %d-spoke gear)" % (plain_letter, cipher_letter, current_spokes)
            #self._show_wheels()
        # Pretty it up into standard four-character block output
        cipher_text = ''.join(cipher_text)
        if spacing:
            cipher_text = [cipher_text[i:i+4] for i in range(0, len(cipher_text), 4)]
            cipher_text = ' '.join(cipher_text)
        return cipher_text
    

    """ Takes a set of prepared wheels, and a ciphertext string, decrypts it """
    def decrypt(self, cipher_text, spacing=False):
        # Ciphertext should already be clean, but let's do this step anyway
        cipher_text = re.sub(r'[^A-Z]', '', cipher_text.upper())
        # Decrypt the message
        # Move through plaintext, a letter at a time
        plain_text = list()
        for msg_index, cipher_letter in enumerate(cipher_text):
            # How far do we need to turn the lead wheel to get to this number?
            # On decryption, input gear is always gear 0
            rotation_teeth = self.gears[self.gears_in_use[0]].index(cipher_letter)
            # Rotate all the wheels by that amount, in alternating directions
            for idx, spokes in enumerate(self.gears_in_use):
                self._rotate(spokes, rotation_teeth * pow(-1, idx % 2))
            # Now read off the decrypted letter, from alternating wheels
            current_spokes = self.gears_in_use[(msg_index % (len(self.gears_in_use) - 1)) + 1]
            plain_letter = self.gears[current_spokes][0]
            plain_text.append(plain_letter)
            #print "Decrypting '%s' -> '%s' (from %d-spoke gear)" % (cipher_letter, plain_letter, current_spokes)
            #show_wheels(wheel_set)
        # Pretty it up into standard four-character block output
        plain_text = ''.join(plain_text)
        if spacing:
            plain_text = [plain_text[i:i+4] for i in range(0, len(plain_text), 4)]
            plain_text = ' '.join(plain_text)
        return plain_text
    

def main():
    # Initialize a GearTranslator
    gt = GearTranslator(2684)
    
    # Set a combination
    gt.select_and_set_wheels([2,4,6,7], "DOGS")
    #gt._show_wheels()
    
    # Set a group of wheels, and a key
    plain_text = "Attack at dawn!"
    print plain_text
    
    #cipher_text = gt.encrypt(plain_text, spacing=True)
    cipher_text = gt.encrypt(plain_text)
    print cipher_text
    
    #plain_text = gt.decrypt(cipher_text, spacing=True)
    plain_text = gt.decrypt(cipher_text)
    print plain_text

if __name__ == "__main__":
    main()
