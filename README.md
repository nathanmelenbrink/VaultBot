# VaultBot
 
VaultBot is a robotic system composed of ArchBot and RailBot. 

RailBot can be set to COMMAND mode or AUTONOMOUS mode. ArchBot operates in the same way for either mode. 

COMMAND mode -- User Inputs: 
	- Address (A1)
	- Independent motor commands (motor, steps)
	- Reset step count to zero (motor, Z)

Sequence for COMMAND (user inputs address G2)
	- Move RailBot from current position to arch 
		- Confirm step skip with encoder
		- Reset steps to 0 
	- Send command for row G to ArchBot 
		- ArchBot moves from current row to row G 

ArchBot sends ACK that encoder agrees with steps

HOME routine 
	- Move RailBot from current position to arch 
		- Confirm step skip with encoder
		- Reset steps to 0 