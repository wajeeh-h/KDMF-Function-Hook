# KMDF Function Hook

If you're unfamilar, this is an educational example of a type of cheat used in many competitive games (think Counter-Strike: Global Offensive, CSGO). It involves the use of a few lines of assembly code to intercept and redirect the execution of a function at the kernel (lowest) level of the operating system (Windows 10/11). This program interceps the function NtQueryCompositionSurfaceStatistics and points to itself instead. From there, one can execute whatever code they would like to. 

In this case, we create a driver (annaKMDF) with the functions write_process_memory(process, source, target, size) and read_process_memory(process, source, target, size) which will allow us to read and modify memory pertaining to a certain process. Using this, one could for instance use the read_process_memory function, point towards csgo.exe at the address 0x4DFFF7C, which currently points towards a list of all entities within a match, then find and highlight the locations of all enemy players. 

A cheat which operates at such a low level is impossible to detect with user-mode uninvasive anti-cheats like CSGO's vac. Only anti-cheats which operate at a similar level notice something amiss (i.e. Battle Eye, Easy Anti Cheat, Vanguard).

```
STATUS:
UNDETECTED (by VAC)
DETECTED (by BE, EAC, and Vanguard)

If you were to find an alternative fuction to hook it would bypass BE and EAC
```
```
Communicates with a usermode program to Read and Write process memory through the Kernel. 
This is a safer than directly hooking the program from usermode, like most externals do.
```

![DRIVER](https://i.ibb.co/Hp02T0Z/image.png)

# TODO
- Encrypt Strings (ie. Xorstr)
- Bypass Detection Vectors:

```c++
// 1. ObjectDirectory
// 2. ServiceDatabase
// 3. Registry Service Database
// 4. Signatures
// 5. Pool-tag scanning
// 6. High entropy symbol blocks

// Disable process notification callbacks
*( ULONG * )( PspNotifyEnableMask ) = 0;          

// Remove loaded driver information from globals
MiProcessLoaderEntry( ( PKLDR_DATA_TABLE_ENTRY )( DriverLdrEntry ), FALSE );

```
