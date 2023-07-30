# KMDF Function Hook

This is an educational example of a type of cheat used in many competitive games (think Counter-Strike: Global Offensive, CSGO). It involves the use of a few lines of assembly code to intercept and redirect the execution of a function at the kernel level of the operating system (Windows 10/11). This program interceps the function NtQueryCompositionSurfaceStatistics and points to itself instead. From there, one can execute whatever code they would like to. 

* As an educational example this repo only includes the driver and NOT the actual user-mode cheat and gui (the bulk of the code) though those are relatively straight forward to create and there are many examples (on my profile and on others) of how to do just that.

In this case, we create a driver (annaKMDF) with the functions write_process_memory(process, source, target, size) and read_process_memory(process, source, target, size) which will allow us to read and modify memory pertaining to a certain process. Using this, one could for instance use the read_process_memory function, point towards csgo.exe at the address 0x4DFFF7C, which currently points towards a list of all entities within a match, then find and highlight the locations of all enemy players. 

A cheat which operates at such a low level is impossible to detect with user-mode uninvasive anti-cheats like CSGO's vac. Only anti-cheats which operate at a similar level notice something amiss (i.e. Battle Eye, Easy Anti Cheat, Vanguard).

  
```
STATUS:
UNDETECTED by VAC
DETECTED by BE, EAC, and Vanguard

If you were to find an alternative fuction to hook it would bypass BE and EAC
```

![DRIVER](https://i.ibb.co/Hp02T0Z/image.png)

DebugView lets one monitor debug outputs from different sources, like drivers. If our message shows up it means the driver has succesfully loaded.  

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
