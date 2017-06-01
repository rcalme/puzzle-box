#!/usr/bin/python
import random
import sys
from termcolor import colored
from cryptex import GearTranslator


def create_random_grid(width, height):
    letters = [x for x in 'ABCDEFGHIJKLMNOPQRSTUVXYZ']
    random.seed(1234)
    grid = []
    for xpos in range(height):
        grid.append([])
        for ypos in range(width):
            grid[xpos].append((random.choice(letters), 'white'))
    return grid


def print_grid(grid, colors=True):
    for row in grid:
        for char in row:
            if colors:
                sys.stdout.write(colored(char[0], char[1]))
            else:
                sys.stdout.write(char)
            sys.stdout.write(' ')
        sys.stdout.write("\n")
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
            mask[xpos].append(' ')

    # Determine where on the full-sized grid, the side A holes will land
    mask = apply_holes(mask, holes, 3, 1, 'A')
    # Flip the holes plate top-to-bottom
    holes.reverse()
    # Determine where on the full-sized grid, the side A holes will land
    mask = apply_holes(mask, holes, 10, 3, 'B')
   
    return mask
    

def apply_holes(mask, holes, x_offset, y_offset, side):
    for row_idx, row in enumerate(holes):
        if row_idx + y_offset >= len(mask):
            continue
        for col_idx, row in enumerate(holes[row_idx]):
            if col_idx + x_offset >= len(mask[row_idx]):
                continue
            if holes[row_idx][col_idx] != ' ':
                x_pos, y_pos = col_idx+x_offset, row_idx+y_offset
                mask[y_pos][x_pos] = side
    return mask
    

def mask_message(grid, mask, side, message):
    masked = []
    msg_idx = 0
    for row_idx, row in enumerate(grid):
        masked.append([])
        for col_idx, col in enumerate(grid[row_idx]):
            if mask[row_idx][col_idx] == side and msg_idx < len(message):
                masked[row_idx].append((message[msg_idx], 'blue'))
                msg_idx += 1
            else:
                masked[row_idx].append(grid[row_idx][col_idx])
    return masked 


def main():
    width = 37
    height = 10

    grid = create_random_grid(width, height)
    mask = create_mask_plate(width, height)

    print_grid(grid)
    print_grid(mask, colors=False)
 
    message_a = 'MAGNETCOMBINATIONCODEISHRJAS'
    message_b = 'WIREUPGRANDCHILDRENBIRTHDAYS'
    message_c = 'KNOCKSHAVEANDAHAIRCUTTWOBITS'

    # Prepare to encrypt
    gt = GearTranslator(2684)

    # Prepare side A
    gt.select_and_set_wheels([2,4,5], "DAD")
    masked_front = mask_message(
        grid,
        mask,
        'A',
        gt.encrypt(message_a)
    )
    print_grid(masked_front)

    # Prepare side B
    gt.select_and_set_wheels([4,3,6], "BEE")
    masked_back = mask_message(
        masked_front,
        mask,
        'B',
        gt.encrypt(message_b)
    )
    print_grid(masked_back)

if __name__ == "__main__":
    main()
