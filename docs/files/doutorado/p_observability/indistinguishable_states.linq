<Query Kind="Statements">
  <Reference>&lt;RuntimeDirectory&gt;\System.ValueTuple.dll</Reference>
  <NuGetReference Version="2.2.3">UltraDES</NuGetReference>
  <Namespace>UltraDES</Namespace>
</Query>

var s = Enumerable.Range(0, 20).Select(ii => new State($"s{ii}", ii == 0 ? Marking.Marked : Marking.Unmarked)).ToArray();



Event n = new Event("n", Controllability.Uncontrollable);
Event a = new Event("a", Controllability.Uncontrollable);
Event b = new Event("b", Controllability.Uncontrollable);
Event p = new Event("p", Controllability.Uncontrollable);
Event f = new Event("f", Controllability.Uncontrollable);
Event d = new Event("d", Controllability.Controllable);
Event c = new Event("c", Controllability.Controllable);

//example 1
var G1 = new DeterministicFiniteAutomaton(new[]
{
	new Transition(s[0], n, s[1]),
	new Transition(s[1], a, s[2]),
	new Transition(s[1], b, s[4]),
	new Transition(s[2], p, s[3]),
	new Transition(s[2], f, s[5]),
	new Transition(s[4], p, s[3]),
	new Transition(s[4], f, s[5]),
	new Transition(s[5], d, s[0]),
	new Transition(s[3], c, s[0]),
	new Transition(s[0], c, s[6]),
	new Transition(s[0], d, s[6])
	
  }, s[0], "G1");


var K1 = new DeterministicFiniteAutomaton(new[]
{
	new Transition(s[0], n, s[1]),
	new Transition(s[1], a, s[2]),
	new Transition(s[1], b, s[4]),
	new Transition(s[2], p, s[3]),
	new Transition(s[2], f, s[5]),
	new Transition(s[4], p, s[3]),
	new Transition(s[4], f, s[5]),
	new Transition(s[5], d, s[0]),
	new Transition(s[3], c, s[0])

  }, s[0], "K1"); 

List<List<Event>> attack_set = new List<List<Event>>();

List<Event> v2 = new[] {a, n}.ToList(); // bd
List<Event> v3 = new[] {p, f}.ToList(); // bd

attack_set.Add(v2);
attack_set.Add(v3);




List<List<Tuple<AbstractState, AbstractState>>> Pi = new List<List<Tuple<AbstractState, AbstractState>>>();

//calcula as relações de indistinguishable states para cada atacante
foreach(var A in attack_set){
	var PiA = ISA_alpha(G1, A);
	Pi.Add(PiA);
}

//agora seleciona os pares com um determinado estado
var state = s[0];
var Pi_0 = new List<Tuple<AbstractState, AbstractState>> ();
foreach(var PiA in Pi){
	foreach(var pair in PiA){
		if((pair.Item1==state) || (pair.Item2==state)){
			Pi_0.Add(pair);
		}
	}
}
//agora seleciona os pares com um determinado estado
state = s[6];
var Pi_3 = new List<Tuple<AbstractState, AbstractState>>();
foreach (var PiA in Pi)
{
	foreach (var pair in PiA)
	{
		if ((pair.Item1 == state) || (pair.Item2 == state))
		{
			Pi_3.Add(pair);
		}
	}
}

var Pi_03 = Pi_0.Union(Pi_3);
var Pi2_03 = new List<Tuple<AbstractState, AbstractState>>();
foreach(var pair in Pi_03){
	var aux = Pi_03.Where(par => par.Item1 == pair.Item2);
	if(aux.Count()>0){
		foreach(var p2 in aux){
			Pi2_03.Add(new Tuple<AbstractState, AbstractState>(pair.Item1, p2.Item2));
		}
		
	}
	
	
}

//var heads = funcao3(G9, v2, out Pi);
Console.WriteLine();
var Pi2 = ISA_alpha(G1, v2);
var Pi3 = ISA_alpha(G1, v3);



//var Pi20 = Pi2.Where(pair => pair.Item1 == s[0] ||  pair.Item2 == s[0]);
//var Pi20 = Pi2.Where(pair => pair.Item1 == s[0] ||  pair.Item2 == s[0]);

//void funcao3()


/*------------------------------------------------- ISA_alpha -----
|  Function ISA_alpha
|
|  Purpose:  THIS FUNCTION OBTAINS THE RELATION OF INDISTINGUISHABLE
|			 STATES WITH RESPECT TO ATTACKER ALPHA, ACCORDING TO DEF. 1.
|			 
|			 STARTING FROM AN AUTOMATON G, IT CREATES A NEW AUTOMATON 
|			 ONLY WITH TRANSITIONS LABELED BY EVENTS IN alpha. IN THIS 
|			 NEW AUTOMATON, A BFS IS PERFORMED IN ORDER TO OBTAIN ALL 
|			 AUTOMATON. THESE PAIR OF STATES FORM THE RELATION OF 
|			 INDISTIGUISHABLE STATES.
|
|  Parameters:
|		H (IN) -- IS THE AUTOMATON FROM WHICH THE PAIRS OF INDISTINGUISHABLE
|				  STATES WILL BE OBTAINED
|		alpha (IN) -- IS A LIST OF EVENTS THAT THE ATTACKER CAN MANIPULATE
|
|  Returns:  IT RETURNS A LIST OF PAIRS OF INDISTINGUISHABLE STATES
*-------------------------------------------------------------------*/

List<Tuple<AbstractState, AbstractState>> ISA_alpha(DeterministicFiniteAutomaton H, List<Event> alpha)
{
	//copy the transitions labeled with events in alpha to a new list of transitions
	List<Transition> new_transitions = new List<Transition>();
	foreach (var tr in H.Transitions)
	{
		if (alpha.Contains(tr.Trigger))
		{
			new_transitions.Add(tr);
		}
	}
	//creates new automaton
	var nG = new DeterministicFiniteAutomaton(new_transitions, H.InitialState, "newG");

	//fake event will be used to create the pairs of the type (q, q)
	Event fake = new Event("fa", Controllability.Uncontrollable);
	foreach (var st in H.States)
	{
		new_transitions.Add(new Transition(st, fake, st));
	}

	var nG2 = new DeterministicFiniteAutomaton(new_transitions, H.InitialState, "newG2");
	
	//shows the new automata
	//nG.ShowAutomaton("nG");
	//nG2.ShowAutomaton("nG2");

	//creates dictionaries, grouping transitions by state destinations and origin, that will make it easier to perform the BFS
	Dictionary<AbstractState, List<Transition>> transitions_by_destination = nG.Transitions.GroupBy(t => t.Destination)
																		.ToDictionary(xi => xi.Key, xi => xi.ToList());
	Dictionary<AbstractState, List<Transition>> transitions_by_origin = nG.Transitions.GroupBy(t => t.Origin)
																		.ToDictionary(xi => xi.Key, xi => xi.ToList());

	var initial_states = nG2.States.Except(transitions_by_destination.Keys);

	List<List<AbstractState>> macrostates = new List<List<AbstractState>>();
	List<AbstractState> states = new List<AbstractState>();
	Queue<AbstractState> states_to_add = new Queue<UltraDES.AbstractState>();
	List<Tuple<AbstractState, AbstractState>> relation = new List<Tuple<AbstractState, AbstractState>>();

	//performs the BFS
	foreach (var ini_sta in nG2.States)
	{
		var current_state = ini_sta;
		states_to_add.Enqueue(current_state);
		while (states_to_add.Any())
		{
			current_state = states_to_add.Dequeue();
			var new_pair = new Tuple<AbstractState, AbstractState>(ini_sta, current_state);
			if(!relation.Contains(new_pair)){
				relation.Add(new_pair);
			}
			if (!states.Contains(current_state))
			{
				states.Add(current_state);
			}

			if (transitions_by_origin.ContainsKey(current_state))
			{

				foreach (var tr in transitions_by_origin[current_state])
				{
					if (!states.Contains(tr.Destination))
					{
						states_to_add.Enqueue(tr.Destination);
					}


				}
			}

		}
		macrostates.Add(states.ToList());
		states.Clear();
		



	}

	foreach (var list in macrostates)
	{
		foreach (var st in list)
		{
			Console.Write(st.ToString() + " ");
		}
		Console.WriteLine();
	}
	
	//Nessa parte a partição é montada. Um dicionário cuja chave é o estado e o valor são os estados que estão na mesma classe que o estado é criado.
	Dictionary<AbstractState, List<AbstractState>> parti = new Dictionary<AbstractState, List<AbstractState>>();
	List<List<AbstractState>> parti2 = new List<List<AbstractState>>();
	List<Tuple<AbstractState, AbstractState>> parti3 = new List<Tuple<AbstractState, AbstractState>>();
	
	foreach(var st in nG2.States){
		parti3.Add(new Tuple<AbstractState, AbstractState>(st, st));
	}
	foreach(var list in macrostates){
		for(int st = 0; st < list.Count(); st++){
			for(int st2 = st; st2 < list.Count(); st2++){
				var new_pair = new Tuple<AbstractState, AbstractState>(list.ElementAt(st), list.ElementAt(st2));
				if(!parti3.Contains(new_pair)){
					parti3.Add(new_pair);
				}
			}
		}
	}
	
	
	foreach (var st in nG2.States)
	{
		parti.Add(st, new[] {st}.ToList());
	}
	foreach (var list in macrostates)
	{
		if (list.Count() > 1)
		{
			foreach (var st in list)
			{
				parti[st].AddRange(list.Except(new[] {st}.ToList()).ToList());
			}
		}
		
	}

	List<AbstractState> aux = new List<AbstractState>();
	List<AbstractState> processed = new List<AbstractState>();
	foreach (var st in nG2.States)
	{
		if (!processed.Contains(st))
		{
			foreach (var list in macrostates)
			{
				if (list.Contains(st))
				{
					aux.AddRange(list);
					processed.AddRange(list);
				}
			}
			parti2.Add(aux.ToList());
			aux.Clear();
		}

	}
	
	Console.WriteLine();
	foreach(var pair in parti3){
		Console.WriteLine("("+pair.Item1.ToString() +", "+pair.Item2.ToString()+")");
	}
	Console.WriteLine();
	foreach (var pair in relation)
	{
		Console.WriteLine("(" + pair.Item1.ToString() + ", " + pair.Item2.ToString() + ")");
	}


	Console.WriteLine("\n");
	foreach (var pair in parti)
	{
		Console.Write(pair.Key.ToString()+ " ->");
		foreach (var st in pair.Value)
		{
			Console.Write(" " + st.ToString());
		}
		Console.WriteLine();
	}
	
	

	Console.WriteLine("\n");
	foreach (var list in parti2)
	
	{
		foreach (var st in list)
		{
			Console.Write(st.ToString() + " ");
		}
		Console.WriteLine();
	}



	return relation;
	
}