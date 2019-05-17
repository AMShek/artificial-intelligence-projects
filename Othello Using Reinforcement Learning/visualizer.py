from game import Game
from players import *

h = Human()
ai = Agent(1,1,0.01)
#使用已经训练网络得到weights
ai.policy_net.load("best.weights")

g = Game()
g.addPlayer(ai)
g.addPlayer(h)
g.run(True)
