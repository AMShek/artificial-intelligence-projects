import itertools
import random
import numpy as np
import nn

#agent
class Agent:
	def __init__(self, q_lr, discount_factor, net_lr = 0.01):
		self.policy_net = nn.NN([64, 128, 128, 64, 64], net_lr)

		# Initialization
		self.epsilon = 0.1

		# Variables for Q-learning
		self.q_lr = q_lr
		self.discount_factor = discount_factor
		self.play_history = []
		self.wins = 0

	def play(self, place_func, board_state, me, log_history = True):
		#每个自己颜色的棋子记为1分
		#每个对方的棋子记为-1分
		input_state = np.apply_along_axis(lambda x: int((x==me and 1) or (x!=0 and -1)),
										  1, board_state.reshape((64, 1))).reshape((64,1))
		made_move = False
		pos = None

		# epsilon greedy to pick random move
		if np.random.random() < self.epsilon:
			positions = list(itertools.product(range(8), repeat = 2))
			random.shuffle(positions)
			while not made_move and positions:
				pos = positions.pop()
				made_move = place_func(*pos)

			# If we can make no move... pass
			if not made_move and not positions:
				return False

		else:
			#out是讲当前s输入到神经网络中得到的Q向量
			out = self.policy_net.getOutput(input_state)
			# Sort the possible moves lowest to highest desire
			positions = [(v,i) for i,v in enumerate(out)]
			positions.sort(key = lambda x: x[0], reverse = True)

			while not made_move and positions:
				# Grab next desired move point
				scalar_play_point = positions.pop()[1]
				# Convert the scalar to a 2D coordinate to play on the board
				pos = scalar_play_point // 8, scalar_play_point % 8
				made_move = place_func(*pos)

			# If we can make no move... pass
			if not made_move and not positions:
				return False

		#写入下棋历史记录中（如果参数表明需要记录才写）
		if log_history:
			self.play_history.append((np.copy(input_state), pos[0]*8 + pos[1]))

		return True

	def updateWeights(self,final_score):
		i = 0
		state, action = self.play_history[i]
		#q是使用神经网络得到的在这一状态上的Q向量
		q = self.policy_net.getOutput(state)
		n_play_history = len(self.play_history)
		#更新Q
		while i < n_play_history:
			i += 1

			# 最早的Q。还没有之前的值，直接赋成final_score
			if i == n_play_history:
				q[action] = final_score

			else:
				state_, action_ = self.play_history[i]
				q_ = self.policy_net.getOutput(state_)
				#Q-learning的action value函数
				#version 1:先不计算reward（记为0）
				#TODO
				#version 2:把目前棋盘上的得分作为reward
				q[action] +=  discount_factor * np.max(q_)

			self.policy_net.backProp(state, self.policy_net.mkVec(q))

			if i != n_play_history:
				action, q = action_, q_


class Human:
	def play(self, place_func, board_state, me, _):
		try:
			pos = map(int, map(str.strip, input().split(" ")))
			place_func(*pos)
			return True
		except ValueError:
			return False


def OneHot(index, dim):
	"""
	Converts an index into a one-hot encoded column vector.
	"""
	a = np.zeros((dim,1))
	a[index] = 1
	return a

