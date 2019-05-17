#coding=utf=8
from pomegranate import *

burglary=DiscreteDistribution({'t':0.001, "f":0.999})

earthquake=DiscreteDistribution({'t':0.002, "f":0.998})

alarm=ConditionalProbabilityTable(
	[
		['t','t','t',0.95],
		['t','t','f',0.05],
		
		['t','f','t',0.94],
		['t','f','f',0.06],
		
		['f','t','t',0.29],
		['f','t','f',0.71],
		
		['f','f','t',0.001],
		['f','f','f',0.999]
	],
	[burglary, earthquake]
)

johncalls=ConditionalProbabilityTable(
	[
		['t','t',0.90],
		['t','f',0.10],
		['f','t',0.05],
		['f','f',0.95]
	],
	[alarm]
)

marycalls=ConditionalProbabilityTable(
	[
		['t','t',0.70],
		['t','f',0.30],
		['f','t',0.01],
		['f','f',0.99]
	],
	[alarm]
)

s0=State(burglary, name='burglary')
s1=State(earthquake, name='earthquake')
s2=State(alarm, name='alarm')
s3=State(johncalls, name='johncalls')
s4=State(marycalls, name='marycalls')

model=BayesianNetwork("Burglary Problem")

model.add_states(s0,s1,s2,s3,s4)

model.add_transition(s0,s2)
model.add_transition(s1,s2)
model.add_transition(s2,s3)
model.add_transition(s2,s4)
model.bake()

"""
print ("Alarm rings:")
print model.probability([None, None,'t', None, None])

print ('Alarm rings when burglary and earthquake both happen:')
print model.probability (['t','t','t',None,None])

print 'The number of edges:'
print model.edge_count()
"""
p1=0
p2=0
p3=0
p4=0
p5=0
p6=0
	
#1 means TRUE
#-1 means FALSE
#0 means UNKNOWN
def prob(*states):
	variablesets_count=1
	variablesets=[]
	
	for state in states:
		if state==0:
			variablesets_count*=2
	
	offset=variablesets_count/2
			
	for i in range (0,variablesets_count):
		variablesets.append([])
		
	proba=0
	
	for j in range(0,5):
		if states[j]==1:
			for variableset in variablesets:
				variableset+='t'
		elif states[j]==-1:
			for variableset in variablesets:
				variableset+='f'
		elif states[j]==0:
			for i in range(0,variablesets_count):
				if (int)(i/offset)%2==0:
					variablesets[i]+='t'
				else:
					variablesets[i]+='f'
			offset/=2
	
	#print variablesets
	
	for variableset in variablesets:
		proba+=model.probability(variableset)
		
	return proba
		
state=[0,0,0,1,1]
print ("P(JohnCalls, MaryCalls)=")
print prob(*state)
print "\n"

state=[1,1,1,1,1]
print ("P(Burglary, Earthquake, Alarm, JohnCalls, MaryCalls)=")
print prob(*state)
print "\n"

state1=[0,0,1,1,1]
state2=[0,0,0,1,1]
print ("P(Alarm | JohnCalls, MaryCalls)=")
print prob(*state1)/prob(*state2)
print "\n"

state1=[-1,0,0,1,-1]
state2=[-1,0,0,0,0]
print ("P(JohnCalls, ¬ MaryCalls | ￢Burglary)=")
print prob(*state1)/prob(*state2)
print "\n"
