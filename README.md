# KMDF Function Hook

If you're unfamilar, this is an educational example of a type of cheat used in many competitive games (think Counter-Strike: Global Offensive, CSGO). It involves the use of a few lines of assembly code to intercept and redirect the execution of a system call at the kernel (lowest) level of the operating system (Windows 10/11).  

- An example of a kernel driver function hook.
- Hooks NtQueryCompositionSurfaceStatistics

```
STATUS:
UNDETECTED (by VAC)
DETECTED (by BE and EAC)

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
