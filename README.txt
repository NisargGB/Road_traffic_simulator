The Program simulates a traffic scenario wherein a traffic signal is present at the middle of the road (a part of a junction).
The configuration file in the root directory namely config.ini guides the simulator in the following fields-
	1). It enumerates the different types of vehicles used in the simulation and characterizes them with their respective properties (Its type, The length of the vehicle along the road,
		width of the vehicle, the maximum speed limit of the vehicle type in the simulation, the default acceleration for that type of vehicle and the denoting character of the vehicle
		on the terminal frame simulation.
	    The sample format of the above enumeration is as follows-
		Vehicle_Type = Truck
		Vehicle_Length = 4
		Vehicle	_Width = 3
		Vehicle_MaxSpeed = 4
		Vehicle_Acceleration = 1
		Vehicle_Denotion = T
		end_v
	
	2). Following this, the body of the configuration file is responsible for the flow of the simulator. The following commands induce corresponding effects in the simulation-
		a). START: Starting point of the body and the simulation.
		b). Signal RED/GREEN : Responsible for turning the traffic signal too RED/GREEN respectively.
		c). Pass [x]: Where [x] denotes an integer which states that the simulation is allowed to run for [x] number of instances
		d). [Vehicle_Type] [color]: Denotes the introduction ofof a vehicle in  the simulation of type [Vehicle_Type] and given color. The vehicle is simply rejected if there is no room
		    a new vehicle to be introduced.
		e). END: Termination of the simulation until the vehicles exit the road.

The simulation consequently opens in an window called "Indian Road Traffic Simulator" and the frames are also displayed on the terminal output.

Format: COP_Simulator config.ini
The config.ini file contains the configuration data.

The extension features added are :
1) The autos at red light search for spaces nearby and tilt in order to enter into the vacant locations and change the lane. The autos therfore are made to stuck at odd angles at the red signal.
2) All the bikes which have red signal in front of them, don't stop at red signal; instead go one step ahead upto the zebra crossing and then wait there till the signal turns back green.
3) As soon as the signal turns green, all the tilted autos get back in the vacant lane and proceed forward with increasing speeds.
4) The overtaking logic is also improved. The maximum overtaking tendency is given to bike which can look into the front lane as well as two lanes up and down to see whether overtaking is possible there or not.

The videos are made at large road_lengths and correspondingly larger vehicle_lengths, max speeds, max accelerations; so as to depict real world scenario. In order to simulate on such larger roads, do comment the printing part on the terminal.
As printing on terminal takes time thus disrupting the frames per second and thus the simulation visual features.