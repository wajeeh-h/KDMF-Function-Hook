# KMDF Function Hook
- An example of a kernel driver function hook.
- UNDETECTED (by VAC)
- DETECTED (by BE and EAC)
- Hooks NtQueryCompositionSurfaceStatistics
- Communicates with a usermode program to Read and Write process memory through the Kernel. This is a safer than directly hooking the program from usermode, like most externals do.

- If you were to find an alternative fuction to hook it would bypass BE and EAC
- Sign with a paid certificate or explot a signed driver (ie. Capcom or iqvw64e.sys)

[hi](https://user-images.githubusercontent.com/110701083/187325298-2fc87d63-75a5-4400-b022-0733c8cfbcaf.png)

