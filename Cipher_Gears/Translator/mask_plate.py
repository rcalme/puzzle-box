#!/usr/bin/python
import random
import sys
import string
from termcolor import colored
from cryptex import GearTranslator

INTERSECT_CHARACTER = u"\u25A0"

def create_random_grid(width, height):
    letters = [x for x in string.ascii_uppercase] #'ABCDEFGHIJKLMNOPQRSTUVWXYZ']
    random.seed(1234)
    grid = []
    for xpos in range(height):
        grid.append([])
        for ypos in range(width):
            grid[xpos].append((random.choice(letters), 'white'))
    return grid


def print_grid(grid, colors=True):
    print "+%s+" % ('-' * (len(grid[0])*2-1))
    for row in grid:
        sys.stdout.write("|")
        for idx, char in enumerate(row):
            if colors:
                sys.stdout.write(colored(char[0], char[1]))
            else:
                sys.stdout.write(colored(char[0], 'white'))
            if idx != len(row)-1:
                sys.stdout.write(' ')
        sys.stdout.write("|\n")
    print "+%s+" % ('-' * (len(grid[0])*2-1))
    sys.stdout.write("\n")


def create_mask_plate(width, height):
    holes = [
        'XXXX     XXXX           ',
        '                    XXXX',
        '             XXXX       ',
        '    XXXX                ',
        '                  XXXX  ',
        ' XXXX                   ',
    ]

    mask = []
    for xpos in range(height):
        mask.append([])
        for ypos in range(width):
            mask[xpos].append((' ', 'white'))

    # Determine where on the full-sized grid, the side A holes will land
    mask = apply_holes(mask, holes, 3, 1, 'A', 'blue')
    # Flip the holes plate top-to-bottom
    holes.reverse()
    # Determine where on the full-sized grid, the side A holes will land
    mask = apply_holes(mask, holes, 10, 3, 'B', 'green')
   
    return mask
    

def apply_holes(mask, holes, x_offset, y_offset, side, color='white'):
    intersect = False
    for row_idx, row in enumerate(holes):
        if row_idx + y_offset >= len(mask):
            continue
        for col_idx, row in enumerate(holes[row_idx]):
            if col_idx + x_offset >= len(mask[row_idx]):
                continue
            if holes[row_idx][col_idx] != ' ':
                x_pos, y_pos = col_idx+x_offset, row_idx+y_offset
                if mask[y_pos][x_pos][0] != ' ':
                    mask[y_pos][x_pos] = (INTERSECT_CHARACTER, 'red')
                    intersect = True
                else:
                    mask[y_pos][x_pos] = (side, color)

    if intersect:
        print "Mask holes would intersect in this configuration!"
        print ""
                
    return mask
    

def mask_message(grid, mask, side, message):
    masked = []
    msg_idx = 0
    for row_idx, row in enumerate(grid):
        masked.append([])
        for col_idx, col in enumerate(grid[row_idx]):
            mask_char = mask[row_idx][col_idx]
            if mask_char[0] == side and msg_idx < len(message):
                masked[row_idx].append((message[msg_idx], mask_char[1]))
                msg_idx += 1
            elif mask_char[0] == INTERSECT_CHARACTER and msg_idx < len(message):
                masked[row_idx].append(mask_char)
                msg_idx += 1
            else:
                masked[row_idx].append(grid[row_idx][col_idx])
    return masked 


def main():
    width = 37
    height = 10

    grid = create_random_grid(width, height)
    print "Grid of random letters (%d x %d)" % (width, height)
    print_grid(grid)

    mask = create_mask_plate(width, height)
    print "Two-sided mask with holes"
    print_grid(mask)
 
    message_a = 'MAGNETCOMBINATIONCODEISHRJAS'
    message_b = 'WIREUPYOURCHILDRENSBIRTHDAYS'

    # Prepare to encrypt
    gt = GearTranslator(2684)

    # Prepare side A
    gt.select_and_set_gears([2,4,5], "BOX")
    masked_front = mask_message(
        grid,
        mask,
        'A',
        gt.encrypt(message_a)
    )
    print "First message applied with holes from side A"
    print_grid(masked_front)

    # Prepare side B
    gt.select_and_set_gears([4,3,6], "PUZ")
    masked_back = mask_message(
        masked_front,
        mask,
        'B',
        gt.encrypt(message_b)
    )
    print "Second message applied with holes from side B"
    print_grid(masked_back)

if __name__ == "__main__":
    main()
