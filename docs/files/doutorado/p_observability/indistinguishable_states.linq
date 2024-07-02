<Query Kind="Statements">
  <Reference>&lt;RuntimeDirectory&gt;\System.ValueTuple.dll</Reference>
  <NuGetReference Version="2.2.3">UltraDES</NuGetReference>
  <Namespace>UltraDES</Namespace>
</Query>

var s = Enumerable.Range(0, 20).Select(ii => new State($"{ii}", ii == 0 ? Marking.Marked : Marking.Unmarked)).ToArray();

List<(DeterministicFiniteAutomaton, DeterministicFiniteAutomaton, List<List<Event>>, List<Event>, String)> examples = new List<(DeterministicFiniteAutomaton, DeterministicFiniteAutomaton, List<List<Event>>, List<Event>, String)> {};


Event a = new Event("a", Controllability.Controllable);
Event b = new Event("b", Controllability.Controllable);
Event c = new Event("c", Controllability.Controllable);
Event d = new Event("d", Controllability.Controllable);
Event e = new Event("e", Controllability.Controllable);
Event f = new Event("f", Controllability.Controllable);
Event g = new Event("g", Controllability.Controllable);
Event h = new Event("h", Controllability.Controllable);

Event n = new Event("n", Controllability.Uncontrollable);
Event t = new Event("t", Controllability.Uncontrollable);
Event u = new Event("u", Controllability.Uncontrollable);
Event v = new Event("v", Controllability.Uncontrollable);
Event w = new Event("w", Controllability.Uncontrollable);
Event y = new Event("y", Controllability.Uncontrollable);
Event z = new Event("z", Controllability.Uncontrollable);
Event x = new Event("x", Controllability.Uncontrollable);


var example1 = (G:
	  	new DeterministicFiniteAutomaton(new[]
		{
			new Transition(s[0], t, s[1]),
			new Transition(s[1], u, s[2]),
			new Transition(s[1], v, s[4]),
			new Transition(s[2], x, s[3]),
			new Transition(s[2], w, s[5]),
			new Transition(s[4], x, s[3]),
			new Transition(s[4], w, s[5]),
			new Transition(s[5], d, s[0]),
			new Transition(s[3], c, s[0]),
			new Transition(s[0], c, s[6]),
			new Transition(s[0], d, s[6])

		}, s[0], "G1"),
		K:
		new DeterministicFiniteAutomaton(new[]
		{
			new Transition(s[0], t, s[1]),
			new Transition(s[1], u, s[2]),
			new Transition(s[1], v, s[4]),
			new Transition(s[2], x, s[3]),
			new Transition(s[2], w, s[5]),
			new Transition(s[4], x, s[3]),
			new Transition(s[4], w, s[5]),
			new Transition(s[5], d, s[0]),
			new Transition(s[3], c, s[0])

		  }, s[0], "K1"),
	 	A: //attack set
		  new List<List<Event>> { new List<Event>{t, u},
								  new List<Event>{w, x}},
		Suo: //unobservable events
		  new List<Event> {},
		Name: "Example 1"
	  );

examples.Add(example1);



//example 3

var example3 = (G:
	new DeterministicFiniteAutomaton(new[]
	{
		new Transition(s[0], a, s[1]),
		new Transition(s[1], u, s[2]),
		new Transition(s[2], x, s[3]),
		new Transition(s[3], y, s[4]),
		new Transition(s[2], z, s[5]),
		new Transition(s[5], n, s[7]),
		new Transition(s[5], e, s[4]),
		new Transition(s[7], b, s[8]),
		new Transition(s[4], b, s[6]),
		new Transition(s[1], b, s[9])


	  }, s[0], "G3"),
	K: 
	new DeterministicFiniteAutomaton(new[]
	{
		new Transition(s[0], a, s[1]),
		new Transition(s[1], u, s[2]),
		new Transition(s[2], x, s[3]),
		new Transition(s[3], y, s[4]),
		new Transition(s[2], z, s[5]),
		new Transition(s[5], n, s[7]),
		new Transition(s[5], e, s[4]),
		new Transition(s[7], b, s[8]),
		new Transition(s[4], b, s[6])

	  }, s[0], "K3"),
	A: //attack set
	new List<List<Event>> { new List<Event>{},
							new List<Event>{u, z},
							new List<Event>{y, x}},
	Suo: //unobservable events
		 new List<Event> { n },
	Name: "Example 3"

	  );

examples.Add(example3);

//example 4

var example4 = (G:
	new DeterministicFiniteAutomaton(new[]
	{
		new Transition(s[0], a, s[1]),
		new Transition(s[1], x, s[2]),
		new Transition(s[2], b, s[3]),
		new Transition(s[0], u, s[4]),
		new Transition(s[4], a, s[5]),
		new Transition(s[5], b, s[6])


	  }, s[0], "G4"),
	K:
	new DeterministicFiniteAutomaton(new[]
	{
		new Transition(s[0], a, s[1]),
		new Transition(s[1], x, s[2]),
		new Transition(s[2], b, s[3]),
		new Transition(s[0], u, s[4]),
		new Transition(s[4], a, s[5])

	  }, s[0], "K4"),
	A: //attack set
	new List<List<Event>> { new List<Event>{u},
							new List<Event>{x}},
	Suo: //unobservable events
		 new List<Event> { },
	Name: "Example 4"

	  );

examples.Add(example4);

//example 5

var example5 = (G:
	new DeterministicFiniteAutomaton(new[]
	{
		new Transition(s[0], a, s[1]),
		new Transition(s[1], b, s[2]),
		new Transition(s[0], u, s[3]),
		new Transition(s[3], x, s[4]),
		new Transition(s[4], a, s[5]),
		new Transition(s[5], b, s[6])


	  }, s[0], "G5"),
	K:
	new DeterministicFiniteAutomaton(new[]
	{
		new Transition(s[0], a, s[1]),
		new Transition(s[1], b, s[2]),
		new Transition(s[0], u, s[3]),
		new Transition(s[3], x, s[4]),
		new Transition(s[4], a, s[5])

	  }, s[0], "K5"),
	A: //attack set
	new List<List<Event>> { new List<Event>{u},
							new List<Event>{x}},
	Suo: //unobservable events
		 new List<Event> { },
	Name: "Example 5"


	  );

examples.Add(example5);

//example 6

var example6 = (G:
	new DeterministicFiniteAutomaton(new[]
	{
		new Transition(s[0], a, s[1]),
		new Transition(s[1], u, s[2]),
		new Transition(s[1], b, s[5]),
		new Transition(s[2], b, s[7]),
		new Transition(s[0], c, s[3]),
		new Transition(s[3], b, s[5]),
		new Transition(s[3], x, s[4]),
		new Transition(s[4], b, s[6]),
		new Transition(s[6], d, s[8]),
		new Transition(s[7], d, s[9])


	  }, s[0], "G6"),
	K:
	new DeterministicFiniteAutomaton(new[]
	{
		new Transition(s[0], a, s[1]),
		new Transition(s[1], u, s[2]),
		new Transition(s[1], b, s[5]),
		new Transition(s[2], b, s[7]),
		new Transition(s[0], c, s[3]),
		new Transition(s[3], b, s[5]),
		new Transition(s[3], x, s[4]),
		new Transition(s[4], b, s[6]),
		new Transition(s[6], d, s[8])

	  }, s[0], "K6"),
	A: //attack set
	new List<List<Event>> { new List<Event>{u},
							new List<Event>{x}},
	Suo: //unobservable events
		 new List<Event> { },
	Name: "Example 6"

	  );

examples.Add(example6);

//example 7

var example7 = (G:
	new DeterministicFiniteAutomaton(new[]
	{
		new Transition(s[0], a, s[1]),
		new Transition(s[1], u, s[2]),
		new Transition(s[2], c, s[3]),
		new Transition(s[3], y, s[4]),
		new Transition(s[4], b, s[0]),
		new Transition(s[1], x, s[5]),
		new Transition(s[5], c, s[6]),
		new Transition(s[6], b, s[7])


	  }, s[0], "G7"),
	K:
	new DeterministicFiniteAutomaton(new[]
	{
		new Transition(s[0], a, s[1]),
		new Transition(s[1], u, s[2]),
		new Transition(s[2], c, s[3]),
		new Transition(s[3], y, s[4]),
		new Transition(s[4], b, s[0]),
		new Transition(s[1], x, s[5]),
		new Transition(s[5], c, s[6])

	  }, s[0], "K7"),
	A: //attack set
	new List<List<Event>> { new List<Event>{u, x},
							new List<Event>{y}},
	Suo: //unobservable events
		 new List<Event> { },
	Name: "Example 7"

	  );

examples.Add(example7);



//example 8

var example8 = (G:
	new DeterministicFiniteAutomaton(new[]
	{
		new Transition(s[0], a, s[1]),
		new Transition(s[1], x, s[2]),
		new Transition(s[0], b, s[4]),
		new Transition(s[4], u, s[2]),
		new Transition(s[2], y, s[3]),
		new Transition(s[2], z, s[5]),
		new Transition(s[5], c, s[6]),
		new Transition(s[3], c, s[7])


	  }, s[0], "G8"),
	K:
	new DeterministicFiniteAutomaton(new[]
	{
		new Transition(s[0], a, s[1]),
		new Transition(s[1], x, s[2]),
		new Transition(s[0], b, s[4]),
		new Transition(s[4], u, s[2]),
		new Transition(s[2], y, s[3]),
		new Transition(s[2], z, s[5]),
		new Transition(s[5], c, s[6])

	  }, s[0], "K8"),
	A: //attack set
	new List<List<Event>> { new List<Event>{u, x},
							new List<Event>{y, z}},
	Suo: //unobservable events
		 new List<Event> { },
	Name: "Example 8"

	  );

examples.Add(example8);

//example 9

var example9 = (G:
	new DeterministicFiniteAutomaton(new[]
	{
		new Transition(s[0], u, s[1]),
		new Transition(s[1], a, s[2]),
		new Transition(s[2], x, s[3]),
		new Transition(s[0], x, s[4]),
		new Transition(s[4], a, s[5]),
		new Transition(s[5], u, s[6]),
		new Transition(s[6], c, s[7]),
		new Transition(s[3], c, s[8])


	  }, s[0], "G9"),
	K:
	new DeterministicFiniteAutomaton(new[]
	{
		new Transition(s[0], u, s[1]),
		new Transition(s[1], a, s[2]),
		new Transition(s[2], x, s[3]),
		new Transition(s[0], x, s[4]),
		new Transition(s[4], a, s[5]),
		new Transition(s[5], u, s[6]),
		new Transition(s[6], c, s[7]),

	  }, s[0], "K9"),
	A: //attack set
	new List<List<Event>> { new List<Event>{u, x}},
	Suo: //unobservable events
		 new List<Event> { },
	Name: "Example 9"

	  );

examples.Add(example9);

//example 10

var example10 = (G:
	new DeterministicFiniteAutomaton(new[]
	{
		new Transition(s[0], u, s[1]),
		new Transition(s[1], a, s[2]),
		new Transition(s[2], x, s[3]),
		new Transition(s[0], x, s[4]),
		new Transition(s[4], a, s[5]),
		new Transition(s[5], u, s[6]),
		new Transition(s[6], c, s[7]),
		new Transition(s[3], c, s[8])


	  }, s[0], "G10"),
	K:
	new DeterministicFiniteAutomaton(new[]
	{
		new Transition(s[0], u, s[1]),
		new Transition(s[1], a, s[2]),
		new Transition(s[2], x, s[3]),
		new Transition(s[0], x, s[4]),
		new Transition(s[4], a, s[5]),
		new Transition(s[5], u, s[6]),
		new Transition(s[6], c, s[7]),

	  }, s[0], "K10"),
	A: //attack set
	new List<List<Event>> { new List<Event> { u},
							new List<Event> { x} },
	Suo: //unobservable events
		 new List<Event> { },
	Name: "Example 9 - b"

	  );

examples.Add(example10);


foreach (var ex in examples) //for each example
{
	List<List<Tuple<AbstractState, AbstractState>>> Pi = new List<List<Tuple<AbstractState, AbstractState>>>();
	List<List<Tuple<AbstractState, AbstractState>>> PiHat = new List<List<Tuple<AbstractState, AbstractState>>>();
	List<List<Tuple<AbstractState, AbstractState>>> PiArrow = new List<List<Tuple<AbstractState, AbstractState>>>();
	List<Tuple<AbstractState, AbstractState>> PiA = new List<Tuple<AbstractState, AbstractState>>();
	List<Tuple<AbstractState, AbstractState>> PiCircle = new List<Tuple<AbstractState, AbstractState>>();
	List<Tuple<AbstractState, AbstractState>> PiHatPrime = new List<Tuple<AbstractState, AbstractState>>();
	List<Tuple<AbstractState, AbstractState>> Pi2 = new List<Tuple<AbstractState, AbstractState>>();
	
	
	foreach (var A in ex.Item3) //for each attacker in the attack set
	{
		var Pi_Alpha = ISA_alpha(ex.Item2, A, ex.Item4);
		var Pi_hat = ISCS(ex.Item2, Pi_Alpha);
		var Pi_arrow = ISIOC(ex.Item2, Pi_hat, A, ex.Item4);
		Pi.Add(Pi_Alpha);
		PiHat.Add(Pi_hat);
		PiArrow.Add(Pi_arrow);
	}

	
	foreach (var pi in PiArrow) //for each attacker in the attack set
	{
		PiA.AddRange(pi);
	}
	PiA = PiA.Distinct().ToList();
	
	PiCircle = IST(PiA);
	
	PiHatPrime = ISCSAC(ex.Item2, PiA, ex.Item3.SelectMany(ev => ev).ToList(),  ex.Item4);
	
	Pi2.AddRange(PiCircle);
	Pi2.AddRange(PiHatPrime);
	Pi2 = Pi2.Distinct().ToList();
	
	Console.WriteLine("\n\n" + ex.Item5);
	
	Console.WriteLine("Pi alpha:" );
	foreach (var pi in Pi)
	{
		var ordered_pi = pi.Where(pair => pair.Item1 != pair.Item2).OrderBy(pair => pair.Item1.ToString()).ThenBy(pair => pair.Item2.ToString()).ToList();
		Console.Write("\\{");
		foreach(var pair in ordered_pi)
		{
			if(pair.Item1 != pair.Item2)
			{
				Console.Write("(" + pair.Item1.ToString() + ", " + pair.Item2.ToString() + ")");	
			}
			if(pair != ordered_pi.Last())
			{
				Console.Write(", ");
			}
		}
		Console.WriteLine("\\}");
	}

	Console.WriteLine("Pi_hat_alpha: ");
	foreach (var pi in PiHat)
	{
		var ordered_pi = pi.Where(pair => pair.Item1 != pair.Item2).OrderBy(pair => pair.Item1.ToString()).ThenBy(pair => pair.Item2.ToString()).ToList();
		Console.Write("\\{");
		foreach (var pair in ordered_pi)
		{
			if (pair.Item1 != pair.Item2)
			{
				Console.Write("(" + pair.Item1.ToString() + ", " + pair.Item2.ToString() + ")");
			}
			if (pair != ordered_pi.Last())
			{
				Console.Write(", ");
			}
		}
		Console.WriteLine("\\}");
	}

	Console.WriteLine("Pi_arrow_alpha: ");
	foreach (var pi in PiArrow)
	{
		var ordered_pi = pi.Where(pair => pair.Item1 != pair.Item2).OrderBy(pair => pair.Item1.ToString()).ThenBy(pair => pair.Item2.ToString()).ToList();
		Console.Write("\\{");
		foreach (var pair in ordered_pi)
		{
			if (pair.Item1 != pair.Item2)
			{
				Console.Write("(" + pair.Item1.ToString() + ", " + pair.Item2.ToString() + ")");
			}
			if (pair != ordered_pi.Last())
			{
				Console.Write(", ");
			}
		}
		Console.WriteLine("\\}");

	}

	Console.WriteLine("Pi_A: ");
	var ordered_piA = PiA.Where(pair => pair.Item1 != pair.Item2).OrderBy(pair => pair.Item1.ToString()).ThenBy(pair => pair.Item2.ToString()).ToList();
	Console.Write("\\{");
	foreach (var pair in ordered_piA)
	{
		if (pair.Item1 != pair.Item2)
		{
			Console.Write("(" + pair.Item1.ToString() + ", " + pair.Item2.ToString() + ")");
		}
		if (pair != ordered_piA.Last())
		{
			Console.Write(", ");
		}
	}
	Console.WriteLine("\\}");

	Console.WriteLine("Pi_Circle: ");
	var ordered_piC = PiCircle.Where(pair => pair.Item1 != pair.Item2).Except(ordered_piA).OrderBy(pair => pair.Item1.ToString()).ThenBy(pair => pair.Item2.ToString()).ToList();
	Console.Write("\\{");
	foreach (var pair in ordered_piC)
	{
		if (pair.Item1 != pair.Item2)
		{
			Console.Write("(" + pair.Item1.ToString() + ", " + pair.Item2.ToString() + ")");
		}
		if (pair != ordered_piC.Last())
		{
			Console.Write(", ");
		}
	}
	Console.WriteLine("\\}");

	Console.WriteLine("Pi_HatPrime: ");
	var ordered_piHP = PiHatPrime.Where(pair => pair.Item1 != pair.Item2).Except(ordered_piA).OrderBy(pair => pair.Item1.ToString()).ThenBy(pair => pair.Item2.ToString()).ToList();
	Console.Write("\\{");
	foreach (var pair in ordered_piHP)
	{
		if (pair.Item1 != pair.Item2)
		{
			Console.Write("(" + pair.Item1.ToString() + ", " + pair.Item2.ToString() + ")");
		}
		if (pair != ordered_piHP.Last())
		{
			Console.Write(", ");
		}
	}
	Console.WriteLine("\\}");

	Console.WriteLine("Pi_2: ");
	var ordered_pi2 = Pi2.Where(pair => pair.Item1 != pair.Item2).OrderBy(pair => pair.Item1.ToString()).ThenBy(pair => pair.Item2.ToString()).ToList();
	Console.Write("\\{");
	foreach (var pair in ordered_pi2)
	{
		if (pair.Item1 != pair.Item2)
		{
			Console.Write("(" + pair.Item1.ToString() + ", " + pair.Item2.ToString() + ")");
		}
		if (pair != ordered_pi2.Last())
		{
			Console.Write(", ");
		}
	}
	Console.WriteLine("\\}");

}
Console.WriteLine();




/*

List<List<Tuple<AbstractState, AbstractState>>> Pi = new List<List<Tuple<AbstractState, AbstractState>>>();

//calcula as relações de indistinguishable states para cada atacante
foreach (var A in attack_set)
{
	var PiA = ISA_alpha(G3, A);
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
var Pi2 = ISA_alpha(G3, v2);
//var Pi3 = ISA_alpha(G1, v3);



//var Pi20 = Pi2.Where(pair => pair.Item1 == s[0] ||  pair.Item2 == s[0]);
//var Pi20 = Pi2.Where(pair => pair.Item1 == s[0] ||  pair.Item2 == s[0]);

//void funcao3()

*/

/*------------------------------------------------- ISCSAC -----
|  Function ISCSAC
|
|  Purpose:  THIS FUNCTION OBTAINS THE RELATION OF INDISTINGUISHABLE
|			 STATES DUE TO A COMMON STATE AFTER CONTINUATIONS, ACCORDING DEF. 3.
|			 
|			 STARTING FROM THE RELATION OF INDISTINGUISHABLE
|  			 STATES, IT CREATES A NEW RELATION  STATES DUE TO A COMMON 
|		 	 STATE AFTER CONTINUATIONS
|
|  Parameters:
|		Pi (IN) -- RELATION OF PAIRS OF INDISTINGUISHABLE STATES
|
|  Returns:  IT RETURNS A LIST OF PAIRS OF INDISTINGUISHABLE STATES
*-------------------------------------------------------------------*/
List<Tuple<AbstractState, AbstractState>> ISCSAC(DeterministicFiniteAutomaton H, List<Tuple<AbstractState, AbstractState>> Pi, List<Event> Sigma_v, List<Event> Sigma_u)
{
	var Sigma_to_remove = Sigma_u.Union(Sigma_v);	
	var Obs = H.Projection(Sigma_to_remove);
	
	var Pi_aux = Pi.ToList();
	var new_pairs = new List<Tuple<AbstractState, AbstractState>>();
	foreach (var pair1 in Pi_aux)
	{
		foreach (var pair2 in Pi_aux)
		{
			if (pair1.Item1 != pair1.Item2 && pair2.Item1 != pair2.Item2 && pair1 != pair2)
			{
				foreach(var st in Obs.States)
				{
					if (pair1.Item1 == pair2.Item1 && st.S.Contains(pair1.Item2) && st.S.Contains(pair2.Item2))
					{
						new_pairs.Add(new Tuple<AbstractState, AbstractState>(pair1.Item2, pair2.Item2));
						new_pairs.Add(new Tuple<AbstractState, AbstractState>(pair2.Item2, pair1.Item2));
					}
				}
				
			}
		}
	}
	/*
	Console.Write("Observer states: ");
	foreach(var mt in Obs.States)
	{
		Console.Write(" 	");
		foreach (var st in mt.S)
		{
			Console.Write(st.ToString() + " ");
		}		
	}
	Obs.ShowAutomaton("Obs");
	*/
	Pi_aux.AddRange(new_pairs);
	var aux = Pi_aux.Distinct().ToList();
	return aux;
	


	
	
	return Pi;
}
	/*------------------------------------------------- IST -----
|  Function IST
|
|  Purpose:  THIS FUNCTION OBTAINS THE RELATION OF INDISTINGUISHABLE
|			 STATES DUE TO TRANSITIVITY, ACCORDING DEF. 5.
|			 
|			 STARTING FROM THE RELATION OF INDISTINGUISHABLE
|  			 STATES, IT CREATES A NEW RELATION  STATES OF INDISTINGUISHABLE 
|			 STATES DUE TO TRANSITIVITY.
|
|  Parameters:
|		Pi (IN) -- RELATION OF PAIRS OF INDISTINGUISHABLE STATES
|
|  Returns:  IT RETURNS A LIST OF PAIRS OF INDISTINGUISHABLE STATES
*-------------------------------------------------------------------*/
List<Tuple<AbstractState, AbstractState>> IST( List<Tuple<AbstractState, AbstractState>> Pi)
{
	var pi_aux = Pi.ToList();
	var new_pairs = new List<Tuple<AbstractState, AbstractState>>();
	
	foreach (var pair1 in pi_aux)
	{
		foreach(var pair2 in pi_aux)
		{
			if(pair1.Item2 == pair2.Item1)
			{
				new_pairs.Add(new Tuple<AbstractState, AbstractState>(pair1.Item1, pair2.Item2));
				new_pairs.Add(new Tuple<AbstractState, AbstractState>(pair2.Item2, pair1.Item1));
			}
		}
	}
	pi_aux.AddRange(new_pairs);
	var aux = pi_aux.Distinct().ToList();
	return aux;
}


	/*------------------------------------------------- ISIOC -----
|  Function ISIOC
|
|  Purpose:  THIS FUNCTION OBTAINS THE RELATION OF INDISTINGUISHABLE
|			 STATES DUE TO IDENTICAL OBSERVABLE CONTINUATIONS, ACCORDING
|			 DEF. 4.
|			 
|			 STARTING FROM AN AUTOMATON G AND THE RELATION OF INDISTINGUISHABLE
|  			 STATES, IT CREATES A NEW RELATION  STATES OF INDISTINGUISHABLE 
|			 STATES DUE TO IDENTICAL OBSERVABLE CONTINUATIONS.
|
|  Parameters:
|		H (IN) -- IS THE AUTOMATON FROM WHICH THE PAIRS OF INDISTINGUISHABLE
|				  STATES WILL BE OBTAINED
|		Pi (IN) -- RELATION OF PAIRS OF INDISTINGUISHABLE STATES
|		alpha (IN) -- IS A LIST OF EVENTS THAT THE ATTACKER CAN MANIPULATE. IF THIS
|					  LIST IS EMPTY, THEN IT MEANS THAT THERE IS NO ATTACKER.
|
|  Returns:  IT RETURNS A LIST OF PAIRS OF INDISTINGUISHABLE STATES
*-------------------------------------------------------------------*/
List<Tuple<AbstractState, AbstractState>> ISIOC(DeterministicFiniteAutomaton H, List<Tuple<AbstractState, AbstractState>> Pi, List<Event> Sigma_v, List<Event> Sigma_u)
{
	var Pi_arrow = Pi.ToList();
	var Sigma_M = new List<AbstractEvent>();
	var pi_aux = new List<Tuple<AbstractState, AbstractState>>();
	var Sigma = H.Events.Except(Sigma_u).Except(Sigma_v);
	
	foreach (var t1 in H.Transitions)
	{
		foreach (var t2 in H.Transitions)
		{
			if(t1.Trigger == t2.Trigger)
			{
				if(Sigma.Contains(t1.Trigger))
				{
					if(t1.Origin != t2.Origin && t1.Destination != t2.Destination)
					{
						Sigma_M.Add(t1.Trigger);
					}
				}
			}
		}
	}
	Sigma_M = Sigma_M.Distinct().ToList();
	
	while (Pi_arrow.Count() != pi_aux.Count())
	{
		Pi_arrow.AddRange(pi_aux);
		pi_aux = Pi_arrow;
		foreach(var ev in Sigma_M)
		{
			for (int i=0; i<Pi_arrow.Count(); i++)
			{
				foreach (var t1 in H.Transitions)
				{
					foreach (var t2 in H.Transitions)
					{
						if (Pi_arrow[i].Item1 == t1.Origin && Pi_arrow[i].Item2 == t2.Origin && t1.Trigger == ev && t2.Trigger == ev && t1 != t2)
						{
							pi_aux.Add(new Tuple<AbstractState, AbstractState>(t1.Destination, t2.Destination));
						}

					}
				}
			}
		}
	}
	return Pi_arrow.Distinct().ToList();
}
	/*------------------------------------------------- ISCS -----
|  Function ISCS
|
|  Purpose:  THIS FUNCTION OBTAINS THE RELATION OF INDISTINGUISHABLE
|			 STATES DUE TO A COMMON STATE, ACCORDING DEF. 2.
|			 
|			 STARTING FROM AN AUTOMATON G AND THE RELATION OF INDISTINGUISHABLE
|  			 STATES WITH RESPECT TO ATTACKER ALPHA, IT CREATES A NEW RELATION
|			 STATES OF INDISTINGUISHABLE STATES DUE TO A COMMON STATE.
|
|  Parameters:
|		H (IN) -- IS THE AUTOMATON FROM WHICH THE PAIRS OF INDISTINGUISHABLE
|				  STATES WILL BE OBTAINED
|		Pi (IN) -- RELATION OF PAIRS OF INDISTINGUISHABLE STATES, ACCORDING 
|				   DEF 1.
|
|  Returns:  IT RETURNS A LIST OF PAIRS OF INDISTINGUISHABLE STATES
*-------------------------------------------------------------------*/
List<Tuple<AbstractState, AbstractState>> ISCS(DeterministicFiniteAutomaton H, List<Tuple<AbstractState, AbstractState>> Pi)
{

	var Pi_aux = Pi.ToList();
	var new_pairs = new List<Tuple<AbstractState, AbstractState>>();
	foreach(var pair1 in Pi_aux)
	{
		foreach(var pair2 in Pi_aux)
		{
			if(pair1.Item1 != pair1.Item2 && pair2.Item1 != pair2.Item2 && pair1 != pair2)
			{
				if(pair1.Item1 == pair2.Item1)
				{
					new_pairs.Add(new Tuple<AbstractState, AbstractState>(pair1.Item2, pair2.Item2));
					new_pairs.Add(new Tuple<AbstractState, AbstractState>(pair2.Item2, pair1.Item2));
				}
			}
		}
	}
	
	Pi_aux.AddRange(new_pairs);
	var aux = Pi_aux.Distinct().ToList();
	return aux;
}
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
|		alpha (IN) -- IS A LIST OF EVENTS THAT THE ATTACKER CAN MANIPULATE. IF THIS
|					  LIST IS EMPTY, THEN IT MEANS THAT THERE IS NO ATTACKER.
|		Suo (IN) -- IS A LIST OF UNOBSERVABLE EVENTS
|  Returns:  IT RETURNS A LIST OF PAIRS OF INDISTINGUISHABLE STATES
*-------------------------------------------------------------------*/

List<Tuple<AbstractState, AbstractState>> ISA_alpha(DeterministicFiniteAutomaton H, List<Event> alpha, List<Event> Suo)
{
	alpha.AddRange(Suo); //althoug unobservable events are not necessarily attacked, they can
						 // cause states to become indistinguishable
	
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
	Dictionary<AbstractState, List<Transition>> transitions_by_destination = nG.Transitions.GroupBy(j => j.Destination)
																		.ToDictionary(xi => xi.Key, xi => xi.ToList());
	Dictionary<AbstractState, List<Transition>> transitions_by_origin = nG.Transitions.GroupBy(j => j.Origin)
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

	
	
	
	


	return relation.Distinct().ToList();
	
}