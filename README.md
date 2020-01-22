# About

DSEPATCH (Driver Signing Enforcement) Patch Utility for swapping `CI` (Code Integrity) global values to manipulate Microsoft Windows into loading our driver without a valid code signing certificate.

## Driver Signing Enforcement

Driver Signing Enforcement is a Microsoft mitigation introduced in Windows Vista that required developers to verify themselves with a certificate authority to obtain what is known as an OV certificate, or an EV certificate to be able to load drivers into the kernel space.  [Official Microsoft CA Authority](https://docs.microsoft.com/en-us/windows-hardware/drivers/dashboard/get-a-code-signing-certificate). 

However, Microsoft upped the bar in Microsoft Windows 10 1607 by requiring that devs use a Extended Validation (EV) certificate to be able to use a driver - Something that is tedius, and for a malicious actor, difficult to obtain legitamently. 

### Solution
As such, actors began to steal valid certificates from vendors (looking @ you HackedTeam), and cheat devs began to abuse what is known as "Bring Your Own Driver" approach - finding and abusing bugs in already signed drivers from vendors such as Intel, Razer to be able to achieve access to physical memory through Read and Write primitives.

With this in mind - I choose to go with the latter approach as there are _plenty_ of drivers out there with bugs that permit this kind of thing to choose from. Using a bug in a previous Intel graphics driver, I used it to read and write the Code Integrity (CI) "g_CiOptions" variable to trick Microsoft Windows 10 into allowing me to load a driver which does not have or does not contain a valid EV / OV certificate.

### Why though?

Its been proven over time that even today - detection/defense for kernel malware for persistence is an open field - in this industry its limited in abuse and has an whole new attack surface / limited defense that I'm keen on using to my own advantage.
