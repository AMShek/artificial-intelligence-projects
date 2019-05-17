"""
neural network is traint here!
"""
from pprint import pprint
import numpy as np
from game import Game
from players import Agent
import matplotlib.pyplot as plt

player = Agent(0.07, 0.99, 0.03)
#another player to learn from
rp = Agent(0, 0)
#match times each epoch
match_times = 10
#Q-learning的training epoch
n_epochs = 5000

player_wins = []
for e in range(n_epochs):
	print("Epoch: %d"%e)

	player.wins = 0
	# Anneal the exploration rate
	player.epsilon = (np.exp(-0.017*e)+0.11)/1.1
	player_gameplay_history = []

	for _ in range(match_times):
		player.play_history = []

		# Initialize a new game
		g = Game()
		g.addPlayer(player)
		# Adds a player that won't log to it's move history
		g.addPlayer(rp, False)
		g.run()

		#一场结束之后，使用这场得到的信息
		#final_score为最后比分，BLACK:WHITE
		final_score = list(g.getScore().items())
		final_score.sort()
		ttl = sum(map(lambda x: x[1], final_score))
		
		# 只记录agent(白子)的历史信息，不记录对手的
		player_score =  (final_score[0][1]/ttl - 0.5)*2
		player.wins += player_score > 0
		player_gameplay_history.append((player.play_history, player_score))
	
	#输出epsilon和当前epoch中agent的胜率
	print(player.epsilon, player.wins/match_times)
	player_wins.append(player.wins)
	for game, score in player_gameplay_history:
		player.play_history = game
		player.updateWeights(score)

#输出神经网络训练后得到的weights
suffix = "linear-0.03"
player.policy_net.save("best-%s.weights"%suffix)
print(sum(player_wins))
with open("%d-%d-%s.csv"%(n_epochs, match_times, suffix), "w") as f:
	f.write("\n".join(map(str, player_wins)))

#绘制胜率的变化
plt.plot(player_wins)
plt.draw()
plt.ioff()
plt.show()
