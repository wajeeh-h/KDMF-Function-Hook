# KMDF Function Hook
```
STATUS:
UNDETECTED (by VAC)
DETECTED (by BE and EAC)
```
- An example of a kernel driver function hook.
- Hooks NtQueryCompositionSurfaceStatistics
```
- Communicates with a usermode program to Read and Write process memory through the Kernel. This is a safer than directly hooking the program from usermode, like most externals do.
```

- If you were to find an alternative fuction to hook it would bypass BE and EAC
- Sign with a paid certificate or explot a signed driver (ie. Capcom or iqvw64e.sys)

![DRIVER](https://i.ibb.co/Hp02T0Z/image.png)
