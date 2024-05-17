<Query Kind="Statements">
  <Reference>&lt;RuntimeDirectory&gt;\System.ValueTuple.dll</Reference>
  <NuGetReference>UltraDES</NuGetReference>
  <Namespace>UltraDES</Namespace>
</Query>

//var plants = new[] { M1, M2, M3, M4, M5 };
//var specs = new[] { e1, e2, e3, e4, e5, e6, e7, e8 };

//DeterministicFiniteAutomaton.FromWmodFile(@"E:\Documentos Dropbox\Dropbox\Doutorado\Plantas linqpad\volvo_cell.wmod", out var plants, out var specs);

IMS(out var plants, out var specs);
//DeterministicFiniteAutomaton.ToWmodFile(@"D:\Dropbox\Doutorado\Plantas linqpad\IMS.wmod", plants, specs);

Console.WriteLine("Monolithic Supervisor:");
var timer = new Stopwatch();
timer.Start();
var sup = DeterministicFiniteAutomaton.MonolithicSupervisor(plants, specs, true);
timer.Stop();

var G = DeterministicFiniteAutomaton.ParallelComposition(plants);
var E = DeterministicFiniteAutomaton.ParallelComposition(specs);
var K = G.ParallelCompositionWith(E);

//var sup_op = sup.ObserverPropertyVerify(sup.Events.Except(sup.UncontrollableEvents).ToArray(), out _);
var proj_sup = sup.Projection(sup.UncontrollableEvents).Minimal;


Console.WriteLine("\tStates: {0}", sup.Size);
Console.WriteLine("\tTransitions: {0}", sup.Transitions.Count());
Console.WriteLine("\tComputation Time: {0}", timer.ElapsedMilliseconds / 1000.0);
Console.WriteLine("\tK States: {0}", K.Size);
Console.WriteLine("\tK Transitions: {0}", K.Transitions.Count());
//Console.WriteLine("\tOP: {0}", sup_op);
Console.WriteLine("\tprojection States: {0}", proj_sup.Size);
Console.WriteLine("\tprojection Transitions: {0}", proj_sup.Transitions.Count());


Console.WriteLine("Modular Local Supervisors:");
var sups = DeterministicFiniteAutomaton.LocalModularSupervisor(plants, specs);

var maior_sup = sups.ElementAt(0);
foreach (var t in sups)
{
	if (t.States.Count() > maior_sup.States.Count())
	{
		maior_sup = t;
	}
}

Console.WriteLine("\tMaior sup States: {0}", maior_sup.Size);
Console.WriteLine("\tMaior sup Transitions: {0}", maior_sup.Transitions.Count());

DeterministicFiniteAutomaton k_inter;
List<DeterministicFiniteAutomaton> ks = new List<DeterministicFiniteAutomaton>();

foreach (var es in specs)
{
	k_inter = es;
	foreach (var p in plants)
	{
		if (es.Events.Intersect(p.Events).Count() > 0)
		{
			k_inter = k_inter.ParallelCompositionWith(p);
		}
	}
	ks.Add(k_inter);
}
var maior_k = ks.ElementAt(0);
foreach (var k in ks)
{
	if (k.States.Count() > maior_k.States.Count())
	{
		maior_k = k;
	}
}


Console.WriteLine("\tMaior k States: {0}", maior_k.Size);
Console.WriteLine("\tMaior k Transitions: {0}", maior_k.Transitions.Count());

foreach (var su in sups)
{
	Console.WriteLine("\tOP: {0}", su.ObserverPropertyVerify(su.Events.Except(su.UncontrollableEvents).ToArray(), out _));
}

List<DeterministicFiniteAutomaton> p_sups = new List<DeterministicFiniteAutomaton>();
var comp_sup = DeterministicFiniteAutomaton.ParallelComposition(sups);
Console.WriteLine("\tComposicao Sups states: {0}", comp_sup.Size);
Console.WriteLine("\tComposicao Sups Transitions: {0}", comp_sup.Transitions.Count());

foreach (var su in sups)
{
	p_sups.Add(su.Projection(su.UncontrollableEvents).Minimal);
}
var maior_proj = p_sups.ElementAt(0);
foreach (var su in p_sups)
{
	if (su.States.Count() > maior_proj.States.Count())
	{
		maior_proj = su;
	}
}

Console.WriteLine("\tMaior proj states: {0}", maior_proj.Size);
Console.WriteLine("\tMaior proj Transitions: {0}", maior_proj.Transitions.Count());



var s_proj = DeterministicFiniteAutomaton.ParallelComposition(p_sups);
Console.WriteLine("\tcomposicao das projecoes states: {0}", s_proj.Size);
Console.WriteLine("\tcomposicao das projecoes Transitions: {0}", s_proj.Transitions.Count());



quantEspecification(out var Eq1, "71", 1);
quantEspecification(out var Eq2, "73", 1);

var Eq = Eq1.ParallelCompositionWith(Eq2);

timer.Start();

var seq_original = sequenceCount2(Eq.ParallelCompositionWith(comp_sup).Trim);
Console.WriteLine("\tTotal de sequencias original: {0}", seq_original);

var seq_proj = sequenceCount2(Eq.ParallelCompositionWith(s_proj).Trim);
Console.WriteLine("\tTotal de sequencias na projeção: {0}", seq_proj);
timer.Stop();

Console.WriteLine("\tComputation Time: {0}", timer.ElapsedMilliseconds / 1000.0);

void IMS(out List<DeterministicFiniteAutomaton> p, out List<DeterministicFiniteAutomaton> spec)
{
	var s = Enumerable.Range(0, 6).Select(i => new State($"s{i}", i == 0 ? Marking.Marked : Marking.Unmarked)).ToArray();
	var e = Enumerable.Range(0, 100).Select(i => new Event($"e{i}", i % 2 != 0 ? Controllability.Controllable : Controllability.Uncontrollable)).ToArray();

	//----------------------------
	// Plants
	//----------------------------

	// C1
	var l1 = new DeterministicFiniteAutomaton(
		new[]
		{
		new Transition(s[0], e[11], s[1]),
		new Transition(s[1], e[12], s[0])
		},
		s[0], "L1");

	// C2
	var l2 = new DeterministicFiniteAutomaton(
		new[]
		{
		new Transition(s[0], e[21], s[1]),
		new Transition(s[1], e[22], s[0])
		},
		s[0], "L2");

	var pd1 = new DeterministicFiniteAutomaton(
		new[]
		{
		new Transition(s[0], e[31], s[1]),
		new Transition(s[1], e[32], s[0])
		},
		s[0], "PD1");
		
	// Milling
	var pd2 = new DeterministicFiniteAutomaton(
		new[]
		{
		new Transition(s[0], e[41], s[1]),
		new Transition(s[1], e[42], s[0])
		},
		s[0], "PD2");


	// Lathe
	var agv1 = new DeterministicFiniteAutomaton(
		new[]
		{
		new Transition(s[0], e[53], s[1]),
		new Transition(s[1], e[51], s[2]),
		new Transition(s[2], e[52], s[3]),
		new Transition(s[3], e[51], s[0])
		},
		s[0], "AGV1");

	var agv2 = new DeterministicFiniteAutomaton(
		new[]
		{
		new Transition(s[0], e[61], s[1]),
		new Transition(s[1], e[63], s[2]),
		new Transition(s[2], e[61], s[3]),
		new Transition(s[3], e[62], s[0])
		},
		s[0], "AGV2");

	
	// MM
	var am = new DeterministicFiniteAutomaton(
		new[]
		{
		new Transition(s[0], e[71], s[1]),
		new Transition(s[1], e[73], s[2]),
		new Transition(s[0], e[73], s[3]),
		new Transition(s[3], e[71], s[2]),
		new Transition(s[2], e[72], s[0])
		},
		s[0], "AM");

	//----------------------------
	// Specifications
	//----------------------------

	// E1
	var e1 = new DeterministicFiniteAutomaton(
		new[]
		{
		new Transition(s[0], e[12], s[1]),
		new Transition(s[1], e[31], s[0])
		},
		s[0], "E1");

	// E2
	var e2 = new DeterministicFiniteAutomaton(
		new[]
		{
		new Transition(s[0], e[22], s[1]),
		new Transition(s[1], e[41], s[0])
		},
		s[0], "E2");

	// E5
	var e5 = new DeterministicFiniteAutomaton(
		new[]
		{
		new Transition(s[0], e[32], s[1]),
		new Transition(s[1], e[51], s[0])
		},
		s[0], "E5");

	// E6
	var e6 = new DeterministicFiniteAutomaton(
		new[]
		{
		new Transition(s[0], e[42], s[1]),
		new Transition(s[1], e[53], s[0])
		},
		s[0], "E6");

	// E3
	var e3 = new DeterministicFiniteAutomaton(
		new[]
		{
		new Transition(s[0], e[52], s[1]),
		new Transition(s[0], e[53], s[1]),
		new Transition(s[1], e[61], s[2]),
		new Transition(s[2], e[62], s[1]),
		new Transition(s[2], e[63], s[1]),
		new Transition(s[1], e[51], s[0])
		},
		s[0], "E3");

	// E7
	var e7 = new DeterministicFiniteAutomaton(
		new[]
		{
		new Transition(s[0], e[52], s[1]),
		new Transition(s[1], e[71], s[0]),
		},
		s[0], "E7");

	// E8
	var e8 = new DeterministicFiniteAutomaton(
		new[]
		{
		new Transition(s[0], e[62], s[1]),
		new Transition(s[1], e[73], s[0]),
		
		},
		s[0], "E8");

	p = new[] { l1, l2, pd1, pd2, agv1, agv2, am }.ToList();
	spec = new[] {e1, e2, e3, e5, e6, e6, e7}.ToList();

}

void quantEspecification(out DeterministicFiniteAutomaton Q, string evento, int quantidade)
{
	var s =
		Enumerable.Range(0, quantidade + 1)
			.Select(i =>
				new State(i.ToString(),
					i == quantidade
						? Marking.Marked
						: Marking.Unmarked)
			).ToArray();

	var ev = new Event(evento, Controllability.Controllable);

	List<Transition> trans = new List<Transition>();

	for (int i = 0; i < quantidade; i++)
	{
		var t = new Transition(s[i], ev, s[i + 1]);
		trans.Add(t);

	}

	Q = new DeterministicFiniteAutomaton(
		trans, s[0], "M1");
}

double sequenceCount2(DeterministicFiniteAutomaton g)
{
	//gets the marked states of g
	var ms = g.MarkedStates.ToArray();

	//if there is more than 1 marked states, ends the function
	if (ms.Count() > 1)
	{
		return 0;
	}

	//gets a list with all transictions of g
	var tr = g.Transitions.ToList();

	//creates a list that stores transictions to be removed from tr
	List<Transition> transitions_to_remove = new List<Transition>();

	//creates a list that stores destination states for the transitions
	List<AbstractState> state_destination = new List<AbstractState>();
	//creates a list that stores origin states for the transictions
	List<AbstractState> state_origin = new List<AbstractState>();

	//adds the marked state to the state destination list. The algorithm starts with the marked state
	state_destination.Add(ms[0]);

	//creates a vector that stores the number of possible sequences from a state that has the same index in destination state list
	List<double> sequences = new List<double>();
	List<double> acumulated = new List<double>();
	//sequences.Add(0); //initialize the first element
	acumulated.Add(0);


	int num_estados = 1; //number of states in the first level, the marked state
	bool marked = true;

	//while there is transitions in the list
	while (tr.Count() > 0)
	{
		int aux = 0; //number of transitions that have origin in the next level

		//for each state in the level
		for (int j = 0; j < num_estados; j++)
		{
			//for each transition in the transition list
			foreach (var t in tr)
			{
				//if the transition destination is equal to state j in the level being analyzed
				if (t.Destination == state_destination.ElementAt(j))
				{
					//if the state j is marked
					if (marked)
					{
						state_origin.Add(t.Origin); //add the state origin of transition t in the origin states list 
						sequences.Add(1); //number of paths from marked state to origin state of t 
						aux++; //increment number of transitions that have origin in the next level
						transitions_to_remove.Add(t); //add t in the list of transitions to be removed
						marked = false;
					}
					else
					{
						state_origin.Add(t.Origin); //add the state origin of transition t in the origin states list 
													//sequences[aux] = acumulated[j]; //number of paths from marked state to origin state of t through state j
						if (sequences.Count() >= aux + 1) { sequences.RemoveAt(aux); }
						sequences.Insert(aux, acumulated.ElementAt(j)); //number of paths from marked state to origin state of t through state j
						aux++; //increment number of transitions that have origin in the next level
						transitions_to_remove.Add(t); ////add t in the list of transitions to be removed
					}

				}
			}

			//for each transition in the list of the transitions to be removed
			foreach (var u in transitions_to_remove)
			{
				//Console.WriteLine(transitions_to_remove.Count);
				tr.Remove(u); //removes it from the transition list
			}
			transitions_to_remove.Clear(); //clears the transitions to be removed list
		}

		List<double> sequences_aux = new List<double>();
		//sequences_aux.Add(0);

		List<AbstractState> estado_sem_dupli = state_origin.Distinct().ToList(); //gets only one occurence of each state in the origin state list

		//for each state in the list of origin states without duplicates
		for (int m = 0; m < estado_sem_dupli.Count(); m++)
		{
			sequences_aux.Add(0);
			//gets the indexes from the origin state list where the state is equal to the state m
			var result = Enumerable.Range(0, state_origin.Count).Where(i => state_origin[i] == estado_sem_dupli.ElementAt(m)).ToList();

			//for each occurence of the state m in the origin state list
			for (int k = 0; k < result.Count(); k++)
			{

				var soma_aux = sequences_aux.ElementAt(m) + sequences.ElementAt(result.ElementAt(k)); //adds the number of paths from marked state to 
																									  //state m through diferent transitions wich have origin in the state m
				if (sequences_aux.Count() >= m + 1) { sequences_aux.RemoveAt(m); }
				sequences_aux.Insert(m, soma_aux);
			}
		}

		state_origin = estado_sem_dupli.ToList(); // updates the origin state list
		estado_sem_dupli.Clear(); //clears the list of inique occurence of states
		sequences = sequences_aux; //updates the vector of sequences
		state_destination = state_origin.ToList(); //updates the state destination list, now the next level becomes the current level

		//for each state in the origin state list
		for (int i = 0; i < state_origin.Count; i++)
		{
			if (acumulated.Count() > i)
			{
				acumulated.RemoveAt(i);
			}
			acumulated.Insert(i, sequences.ElementAt(i));//updates the number of paths from marked state to state i

		}

		num_estados = state_origin.Count; //gets the number of states in the current level
		state_origin.Clear(); //clears the state origin list

	}

	return sequences[0]; //returns the number of paths from marked state to inicial state
}