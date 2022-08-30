# KMDF Function Hook
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
// Disable process notification callbacks
*( ULONG * )( PspNotifyEnableMask ) = 0;          

// Remove loaded driver information from globals
MiProcessLoaderEntry( ( PKLDR_DATA_TABLE_ENTRY )( DriverLdrEntry ), FALSE );

```
