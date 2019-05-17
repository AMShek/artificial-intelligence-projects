#coding=utf=8
from pomegranate import *

values_pa=['0-30', '31-65', '65+']
PatientAge=DiscreteDistribution({'0-30':0.10, '31-65':0.30,'65+':0.60 })

values_ct=['Ischemic','Hemmorraghic']
CTScanResult=DiscreteDistribution({'Ischemic':0.70,'Hemmorraghic':0.30})

values_mri=['Ischemic','Hemmorraghic']
MRIScanResult=DiscreteDistribution({'Ischemic':0.70,'Hemmorraghic':0.30})

values_anti=['Used', 'Not used']
Anticoagulants=DiscreteDistribution({'Used':0.50,'Not used':0.5})

values_stroke=['Ischemic','Hemmorraghic','Mimic']
StrokeType=ConditionalProbabilityTable(
	[
		['Ischemic','Ischemic','Ischemic',0.8],
		['Ischemic', 'Hemmorraghic','Ischemic',0.5],
		['Hemmorraghic', 'Ischemic', 'Ischemic',0.5],
		['Hemmorraghic', 'Hemmorraghic', 'Ischemic', 0],
		
		['Ischemic', 'Ischemic', 'Hemmorraghic', 0],
		['Ischemic', 'Hemmorraghic', 'Hemmorraghic',0.4],
		[ 'Hemmorraghic', 'Ischemic', 'Hemmorraghic', 0.4],
		[ 'Hemmorraghic', 'Hemmorraghic', 'Hemmorraghic',0.9],
		
		['Ischemic', 'Ischemic', 'Mimic', 0.2],
		['Ischemic', 'Hemmorraghic', 'Mimic', 0.1],
		[ 'Hemmorraghic', 'Ischemic', 'Mimic', 0.1],
		['Hemmorraghic', 'Hemmorraghic', 'Mimic', 0.1]
	],
	[CTScanResult, MRIScanResult]
)

values_mortal=['True', 'False']
Mortality=ConditionalProbabilityTable(
	[
		['Ischemic','Used','False',0.28],
		['Ischemic','Used','True',0.72],
		
		['Ischemic', 'Not used','False',0.56],
		['Ischemic', 'Not used','True',0.44],
		
		['Hemmorraghic', 'Used','False',0.99],
		['Hemmorraghic', 'Used','True', 0.01],
		
		['Hemmorraghic', 'Not used','False',0.58],
		['Hemmorraghic', 'Not used','True', 0.42],
		
		['Mimic', 'Used','False',0.1],
		['Mimic', 'Used','True', 0.9],
		
		['Mimic', 'Not used','False',0.05],
		['Mimic', 'Not used','True', 0.95],
	],
	[StrokeType, Anticoagulants]
)

values_dis=['Negligible', 'Moderate', 'Severe']
Disability=ConditionalProbabilityTable(
	[
		['Ischemic',	'0-30',	'Negligible',0.80],
		['Hemmorraghic','0-30',	'Negligible',0.70],
		['Mimic', 		'0-30',	'Negligible',0.90],
		['Ischemic',	'31-65','Negligible',0.60],
		['Hemmorraghic','31-65','Negligible',0.50],
		['Mimic', 		'31-65','Negligible',0.40],
		['Ischemic',	'65+',	'Negligible',0.30],
		['Hemmorraghic','65+',	'Negligible',0.20],
		['Mimic', 		'65+',	'Negligible',0.10],
		
		['Ischemic',	'0-30',	'Moderate',0.10],
		['Hemmorraghic','0-30',	'Moderate',0.20],
		['Mimic', 		'0-30',	'Moderate',0.05],
		['Ischemic',	'31-65','Moderate',0.30],
		['Hemmorraghic','31-65','Moderate',0.40],
		['Mimic', 		'31-65','Moderate',0.30],
		['Ischemic',	'65+',	'Moderate',0.40],
		['Hemmorraghic','65+',	'Moderate',0.20],
		['Mimic', 		'65+',	'Moderate',0.10],
		
		['Ischemic',	'0-30',	'Severe',0.10],
		['Hemmorraghic','0-30',	'Severe',0.10],
		['Mimic', 		'0-30',	'Severe',0.05],
		['Ischemic',	'31-65','Severe',0.10],
		['Hemmorraghic','31-65','Severe',0.10],
		['Mimic', 		'31-65','Severe',0.30],
		['Ischemic',	'65+',	'Severe',0.30],
		['Hemmorraghic','65+',	'Severe',0.60],
		['Mimic', 		'65+',	'Severe',0.80]
	],
	[StrokeType, PatientAge]
)

s1=State(PatientAge, name='PatientAge')
s2=State(CTScanResult, name='CTScanResult')
s3=State(MRIScanResult, name='MRIScanResult')
s4=State(Anticoagulants, name='Anticoagulants')
s5=State(StrokeType, name='StrokeType')
s6=State(Mortality, name='Mortality')
s7=State(Disability, name='Disability')

model=BayesianNetwork("Diagonosing Problem")

model.add_states(s1,s2,s3,s4,s5,s6,s7)

model.add_transition(s1,s7)
model.add_transition(s5,s7)

model.add_transition(s2,s5)
model.add_transition(s3,s5)

model.add_transition(s4,s6)
model.add_transition(s5,s6)
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
#################p1#################
p_a=0
p_b=0

for value_mri in values_mri:
	for value_anti in values_anti:
		for value_stroke in values_stroke:
			for value_dis in values_dis:
				p_a+=model.probability(['0-30', 'Ischemic', value_mri, value_anti, value_stroke, 'True', value_dis])
				for value_mortal in values_mortal:
					p_b+=model.probability(['0-30', 'Ischemic', value_mri, value_anti, value_stroke, value_mortal, value_dis])

print ("P(Mortality='True' | PatientAge='0-30' , CTScanResult='Ischemic Stroke')=")
print p_a/p_b
print "\n"
################p2#################
p_a=0
p_b=0

for value_ct in values_ct:
	for value_anti in values_anti:
		for value_stroke in values_stroke:
			for value_mortal in values_mortal:
				p_a+=model.probability(['65+', value_ct, 'Ischemic', value_anti, value_stroke, value_mortal, 'Severe'])
				for value_dis in values_dis:
					p_b+=model.probability(['65+', value_ct, 'Ischemic', value_anti, value_stroke, value_mortal, value_dis])

print ("P(Disability=' Severe ' | PatientAge='65+' , MRIScanResult=' Ischemic Stroke ')=")
print p_a/p_b
print "\n"
################p3#################
p_a=0
p_b=0

for value_anti in values_anti:
	for value_mortal in values_mortal:
		for value_dis in values_dis:
			p_a+=model.probability(['65+', 'Hemmorraghic', 'Ischemic', value_anti, 'Mimic', value_mortal, value_dis])
			for value_stroke in values_stroke:
				p_b+=model.probability(['65+', 'Hemmorraghic', 'Ischemic', value_anti, value_stroke, value_mortal, value_dis])

print ("P(StrokeType='Stroke Mimic' | PatientAge='65+' , CTScanResult='Hemmorraghic Stroke' , MRIScanResult='Ischemic Stroke')=")
print p_a/p_b
print "\n"
################p4#################
p_a=0
p_b=0

for value_ct in values_ct:
	for value_mri in values_mri:
		for value_dis in values_dis:
			p_a+=model.probability(['0-30', value_ct, value_mri, 'Used', 'Mimic', 'False', value_dis])
			for value_mortal in values_mortal:
				p_b+=model.probability(['0-30', value_ct, value_mri, 'Used', 'Mimic', value_mortal, value_dis])


print ("P(Mortality='False' | PatientAge='0-30', Anticoagulants=’Used’, StrokeType='Stroke Mimic')=")
print p_a/p_b
print "\n"

################p5#################

print ("P( PatientAge='0-30', CTScanResult='Ischemic Stroke', MRIScanResult=' 'Hemmorraghic Stroke' , Anticoagulants=’Used’, StrokeType='Stroke Mimic' , Disability=' Severe' , Mortality='False' )=")
print model.probability(['0-30', 'Ischemic','Hemmorraghic','Used','Mimic','False','Severe'])
print "\n"

