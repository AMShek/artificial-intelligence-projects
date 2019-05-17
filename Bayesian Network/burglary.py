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

s1=State(burglary, name='burglary')
s2=State(earthquake, name='earthquake')
s3=State(alarm, name='alarm')
s4=State(johncalls, name='johncalls')
s5=State(marycalls, name='marycalls')

model=BayesianNetwork("Burglary Problem")

model.add_states(s1,s2,s3,s4,s5)

model.add_transition(s1,s3)
model.add_transition(s2,s3)
model.add_transition(s3,s4)
model.add_transition(s3,s5)
model.bake()

values=['t','f']

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
	

for value_b in values:
	for value_e in values:
		for value_a in values:
			p1+=model.probability([value_b, value_e, value_a, 't', 't']) 
print ("P(JohnCalls, MaryCalls)=")
print p1
print "\n"

p2=model.probability(['t','t','t','t','t'])
print ("P(Burglary, Earthquake, Alarm, JohnCalls, MaryCalls)=")
print p2
print "\n"

for value_b in values:
	for value_e in values:
		p3+=model.probability([value_b, value_e, 't', 't', 't'])
for value_b in values:
		for value_e in values:
			for value_a in values:
				p4+=model.probability([value_b, value_e, value_a,'t','t'])
print ("P(Alarm | JohnCalls, MaryCalls)=")
print p3/p4
print "\n"

for value_e in values:
	for value_a in values:
		p5+=model.probability(['f', value_e, value_a, 't', 'f'])
for value_e in values:
	for value_a in values:
		for value_j in values:
			for value_m in values:
				p6+=model.probability(['f', value_e, value_a, value_j, value_m])
print ("P(JohnCalls, ¬ MaryCalls | ￢Burglary)=")
print p5/p6
print "\n"
