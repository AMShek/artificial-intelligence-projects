import sys
from collections import defaultdict
import numpy as np
from colorama import init, Fore, Back, Style
init(autoreset=True)


class Board(object):
	BLACK = 1
	WHITE = -1

	def __init__(self):
		#board用一个矩阵存放
		self.board = np.zeros((8,8), int)
		self.board[3][3] = Board.BLACK
		self.board[4][4] = Board.BLACK
		self.board[4][3] = Board.WHITE
		self.board[3][4] = Board.WHITE

		self.remaining_squares = 8*8 - 4
		#成员变量score用map数据类型存放
		self.score = {Board.BLACK: 2, Board.WHITE: 2}

	def getScore(self):
		return self.score

	def getState(self):
		return self.board

	def isOnBoard(self, x, y):
		return x >= 0 and x <= 7 and y >= 0 and y <= 7

#先判断某一步是否合法
#若合法，更新棋盘
#若非法，不更新
	def updateBoard(self, tile, row, col):
		result = self.isValidMove(tile, row, col)
		
		if result:
			# Flip the disks
			self.board[row][col] = tile
			for row in result:
				self.board[row[0]][row[1]] = tile

			# Update the players' scores
			self.score[tile] += len(result) + 1

			# The gross expression is a mapping for -1 -> 1 and 1 -> -1
			# Rescales the range to [0,1] then mod 2 then rescale back to [-1,1]
			self.score[(((tile+1)//2+1)%2)*2-1] -= len(result)

			# Number of open squares decreases by 1
			self.remaining_squares -= 1

			return True

		else:
			return False

#沿着8种可能的方向判断某一步棋是否合法（是否会引发reversion）
#返回值为造成的reversion的数量
	def isValidMove(self, tile, xstart, ystart):
		if not self.isOnBoard(xstart, ystart) or self.board[xstart][ystart] != 0:
			return False

		# temporarily set the tile on the board.
		self.board[xstart][ystart] = tile

		otherTile = tile * -1

		tiles_to_flip = []
		# loop through all directions around flipped tile
		for xdirection, ydirection in ((0,1),(1,1),(1,0),(1,-1),(0,-1),(-1,-1),(-1,0),(-1,1)):
			x, y = xstart, ystart
			x += xdirection # first step in the direction
			y += ydirection # first step in the direction
			if self.isOnBoard(x, y) and self.board[x][y] == otherTile:
				# There is a piece belonging to the other player next to our piece.
				x += xdirection
				y += ydirection
				if not self.isOnBoard(x, y):
					continue
				while self.board[x][y] == otherTile:
					x += xdirection
					y += ydirection
					if not self.isOnBoard(x, y):
						# break out of while loop, then continue in for loop
						break
				if not self.isOnBoard(x, y):
					continue
				if self.board[x][y] == tile:
					# There are pieces to flip over. Go in the reverse direction
					# until we reach the original space, noting all the tiles
					# along the way.
					while True:
						x -= xdirection
						y -= ydirection
						if x == xstart and y == ystart:
							break
						tiles_to_flip.append([x, y])

		# restore the empty space
		self.board[xstart][ystart] = 0

		# If no tiles were flipped, this is not a valid move.
		return tiles_to_flip

	def printBoard(self):
		def getItem(item):
			if item == Board.BLACK :
				return Fore.WHITE + "|" + Fore.BLACK + "X"
			elif item == Board.WHITE :
				return Fore.WHITE + "|" + Fore.WHITE + "O"
			else:
				return Fore.WHITE + "| "

		def getRow(row):
			return "".join(map(getItem,row))

		#print("\t" + Back.BLACK +              "      BOARD      ")
		print ()
		print("\t" + Back.GREEN + Fore.WHITE + " |0|1|2|3|4|5|6|7")
		for i in range(8):
			print("\t" + Back.GREEN + Fore.WHITE + "{}{}".format(i,
				getRow(self.board[i])))
			sys.stdout.write(Style.RESET_ALL)
