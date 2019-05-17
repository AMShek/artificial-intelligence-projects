class VariableElimination:
	@staticmethod
	def printFactors(factorList):
		for factor in factorList:
			factor.printInf()
	@staticmethod
	def inference(factorList, queryVariables, 
	orderedListOfHiddenVariables, evidenceList):
		for ev in evidenceList:
			to_append=[]
			to_remove=[]
			for factor in factorList:
				#factor.printInf()
				if ev in factor.varList:
					#if this factor has only 1 variable
					#it is removed after restriction
					if len(factor.varList)==1:
						factorList.remove(factor)
						continue
					factor_new=factor.restrict(ev,evidenceList[ev])
					to_append.append(factor_new)
					to_remove.append(factor)
					"""
					appending and removing within loop causes chaos!
					factorList.append(factor_new)
					factorList.remove(factor)
					"""
			for append in to_append:
				factorList.append(append)
			for remove in to_remove:
				factorList.remove(remove)

		for var in orderedListOfHiddenVariables:
			flag=0
			g=Node(var,[var])
			remove_list=[]
			for factor in factorList:
				if var in factor.varList:
					if flag==0:
						g=factor
						flag=1
						remove_list.append(factor)
					else:
						g=g.multiply(factor)
						remove_list.append(factor)
					
			for remove_factor in remove_list:
				factorList.remove(remove_factor)
			factorList.append(g.sumout(var)) 
		res = factorList[0]
		for factor in factorList[1:]:
			res = res.multiply(factor)
		total = sum(res.cpt.values())
		res.cpt = {k: v/total for k, v in res.cpt.items()}
		return res
class Util:
	@staticmethod
	def to_binary(num, len):
		return format(num, '0' + str(len) + 'b')
class Node:
	def __init__(self, name, var_list):
		self.name = name
		self.varList = var_list
		self.cpt = {}
	def setCpt(self, cpt):
		self.cpt = cpt
	def printInf(self):
		print ("Name = " + self.name)
		print (" vars " + str(self.varList))
		for key in self.cpt:
			print ("   key: " + key + " val : " + str(self.cpt[key]))
		print()
	def multiply (self,factor):
		"""
		multiply with another factor
		CODE HERE
		"""
		#the new varList the union of 2 old varLists
		new_var_list=[]
		new_cpt={}
		cpt_ref={}
		common=[]
		common_self_index=[]
		common_factor_index=[]
		distinct_self=[]
		distinct_factor=[]

		common=[variable for variable in factor.varList if variable in self.varList]
		common_self_index=[self.varList.index(variable) for variable in self.varList if variable in factor.varList]
		common_factor_index=[factor.varList.index(variable) for variable in factor.varList if variable in self.varList]
		distinct_self=[variable for variable in self.varList if variable not in factor.varList]
		distinct_factor=[variable for variable in factor.varList if variable not in self.varList]

		for key in self.cpt:
			key_common=''.join([key[i] for i in range(len(self.varList)) if i in common_self_index])
			key_self=''.join([key[i] for i in range(len(self.varList)) if i not in common_self_index])
			if key_common not in cpt_ref:
				cpt_ref[key_common]={}
			cpt_ref[key_common][key_self]=self.cpt[key]

		for key in factor.cpt:
			key_common=''.join([key[i] for i in range(len(factor.varList)) if i in common_factor_index])
			key_factor=''.join([key[i] for i in range(len(factor.varList)) if i not in common_factor_index])
			key_temp=cpt_ref[key_common]
			for key2 in key_temp:
				key_combined=key_common+key2+key_factor
				#print("combined")
				#print (key_combined)
				new_cpt[key_combined]=key_temp[key2]*(factor.cpt[key])

		#print (new_cpt)

		new_var_list=common+distinct_self+distinct_factor

		new_node=Node("f"+str(new_var_list),new_var_list)
		new_node.setCpt(new_cpt)
		return new_node
	def sumout(self, variable):
		"""function that sums out a variable given a factor"""
		new_var_list=[]
		new_cpt={}
		for var in self.varList:
			if var!=variable:
				new_var_list.append(var)
		pos=self.varList.index(variable)
		n=len(self.varList)
		for i in range(2**(n-1)):
			p1=i+int(i/(2**(n-pos-1)))
			p2=i+int(i/(2**(n-pos-1)))+2**(n-pos-1)
			t1=Util.to_binary(int(p1),n)
			t2=Util.to_binary(int(p2),n)
			new_cpt[Util.to_binary(int(i),n-1)]=self.cpt[t1]+self.cpt[t2]
		new_node = Node("f" + str(new_var_list), new_var_list)
		new_node.setCpt(new_cpt)
		return new_node
	def restrict(self, variable, value):
		"""function that restricts a variable to some value 
		in a given factor"""
		new_var_list=[]
		new_cpt={}

		if variable in self.varList and len(self.varList)==1:
			empty_node=Node(variable,[variable])
			empty_node.setCpt({Util.to_binary(int(value),1):1,Util.to_binary(int(1-value),1):0})
			return empty_node

		index =self.varList.index(variable)

		new_var_list=self.varList[:index]+self.varList[index+1:]

		for key in self.cpt:
			new_cpt[key[:index]+key[index+1:]]=self.cpt[key[:index]+str(value)+key[index+1:]]

		new_node=Node("f" + str(new_var_list), new_var_list)
		new_node.setCpt(new_cpt)
		return new_node
# create nodes for Bayes Net
B = Node("B", ["B"])
E = Node("E", ["E"])
A = Node("A", ["A", "B","E"])
J = Node("J", ["J", "A"])
M = Node("M", ["M", "A"])

# Generate cpt for each node
B.setCpt({'0': 0.999, '1': 0.001})
E.setCpt({'0': 0.998, '1': 0.002})
A.setCpt({'111': 0.95, '011': 0.05, '110':0.94,'010':0.06,
'101':0.29,'001':0.71,'100':0.001,'000':0.999})
J.setCpt({'11': 0.9, '01': 0.1, '10': 0.05, '00': 0.95})
M.setCpt({'11': 0.7, '01': 0.3, '10': 0.01, '00': 0.99})

print ("P(Alarm)=")
print(VariableElimination.inference([B,E,A,J,M], ['A'], ['B', 'E', 'J','M'], {}).cpt['1'])
print()

print ("P(J&&~M)=")
print(VariableElimination.inference([B,E,A,J,M], ['J','M'], ['B','E','A'], {}).cpt['10'])
print()

print ("P(A|J&&~M)=")
print(VariableElimination.inference([B,E,A,J,M], ['A'], ['B','E'], {'J':1,'M':0}).cpt['1'])
print()

print ("P(B|A)=")
print(VariableElimination.inference([B,E,A,J,M], ['B'], ['E','J','M'], {'A':1}).cpt['1'])
print()

print ("P(B|J~M)=")
print(VariableElimination.inference([B,E,A,J,M], ['B'], ['E','A'], {'J':1,'M':0}).cpt['1'])
print()

print ("P(J&&~M|~B)=")
print(VariableElimination.inference([B,E,A,J,M], ['J','M'], ['E','A'], {'B':0}).cpt['10'])
print()
