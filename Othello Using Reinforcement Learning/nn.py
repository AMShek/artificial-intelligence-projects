import math
import numpy as np
import random
#用于读写文件
import pickle


class NN:
	#初始化神经网络
	#参数layer_dims指定网络的层数和每一层的结点数量，显然8*8的Othllo输入层应有64个结点
	#参数learning_rate,在反向传播算法计算校正值delta的时候要用到
	def __init__(self, layer_dims, learning_rate):
		self.learning_rate = learning_rate
		self.layer_dims = layer_dims
		self.layers = []
		#用高斯分布的概率密度函数初始化网络
		for i in range(len(layer_dims)-1):
			self.layers.append(np.random.normal(0, 1, size=(layer_dims[i+1], layer_dims[i]+1)))

		self.activation_func  = None
		self.dactivation_func = None

	#将训练得到的weights输出到文件
	def save(self, filename):
		with open(filename, "wb") as f:
			pickle.dump(self.layers, f)

	#读weights文件
	def load(self, filename):
		with open(filename, "rb") as f:
			self.layers = pickle.load(f)

	#将矩阵转换成对应的向量
	def mkVec(self, vector1D, add_bias = True):
		return np.reshape(vector1D, (len(vector1D), 1))

	#输出某一状态s下的Q向量Q(s)
	def getOutput(self, input_vector):
		outputs = input_vector
		for i in range(len(self.layers)):
			outputs = activation(self.layers[i]@np.vstack((outputs, 1)))
		return outputs
	
	#反向传播
	def backProp(self, sample, target):
		# Propagate forwards to get the network's layers' outputs
		outputs = [sample]
		for i in range(len(self.layers)):
			outputs.append(activation(self.layers[i].dot(np.vstack((outputs[i], 1)))))

		# These will still need to be multiplied by the output from the previous layer
		# e.g. layer_deltas[0]*outputs[-2]
		layer_deltas = np.empty(len(outputs) - 1, object)

		# 输出层
		layer_deltas[-1] = (target - outputs[-1]) * dactivation(outputs[-1])


		# i == current layer; Walk backwards from second to last layer (Hence
		# start at -2, because len-1 is the last element) Also recall that
		# range "end" is exclusive.
		for i in range(len(layer_deltas) - 2, -1, -1):
			# Need to do i+1 because outputs[0] == the input sample, and i+1 is
			# the ith layer's output
			layer_derivative = dactivation(outputs[i+1])

			# 校正值delta
			layer_deltas[i] = layer_derivative * (self.layers[i+1].T.dot(layer_deltas[i + 1])[:-1])

		for i in range(len(self.layers)):
			# Because outputs[0] == input sample, layer[i] input == outputs[i]
			# This is delta_weights
			self.layers[i] += self.learning_rate * np.c_[outputs[i].T, 1] * layer_deltas[i]

		return outputs[-1]



#激活函数为双曲正切函数
def activation(x):
	return np.tanh(x)


def dactivation(x):
	return 1 - np.tanh(x)**2
