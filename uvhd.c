/* uvhd.c - binary file investigation utility written in ANSI C             */
/*        - this is the ANSI 'C' source program for Unix/Linux/Windows      */
/*        - written by Owen Townsend, UV Software, owen@uvsoftware.ca       */
/*        - Copyright(C) 1993-2020, UV Software Inc, distributed under GPLv3*/
/*                                                                          */
/*                    ** uvhd - Program Description **                      */
/*                                                                          */
/* 'uvhd' is a binary file investigation utility. It displays any file in   */
/* vertical hexadecimal and prompts for commands to browse, search, select, */
/* update, scan/replace, print, translate, etc. uvhd is an interactive      */
/* utility with a command line interface and 26 help screens.               */
/*                                                                          */
/*                     ** Licensed under GPLv3 **                           */
/*                                                                          */
/* This program is free software: you may redistribute it and/or modify     */
/* it under the terms of GPLv3 (GNU General Public License version 3),      */
/* as published by the Free Software Foundation.                            */
/*                                                                          */
/* This program is distributed in the hope that it will be useful,          */
/* but WITHOUT ANY WARRANTY; without even the implied warranty of           */
/* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.                     */
/*                                                                          */
/* See the full description of the GNU General Public License at:           */
/* http://www.gnu.org/licenses.                                             */
/*                                                                          */
/* Note - for Unix/Linux you must download the source & compile on your OS  */
/*      - see Compile Instructions about 100 lines below                    */
/*                                                                          */
/* Note - for Windows or DOS you may download the precompiled execcutable   */
/*      - store in program files or setup environment PATH to your download */
/*                                                                          */
/* uvhd is an efective tool for investigating problems and verifying test   */
/* results in files that have packed/binary fields, and that may not have   */
/* linefeeds. Most Unix/Linix tools require linefeeds (vi, cat, lp, etc).   */
/*                                                                          */
/* 'uvhd' is offered as a Free sample of the Vancouver Utilties which are   */
/* often used to convert mainframe JCL, COBOL,& DATA to Unix/Linux/Windows  */
/* Mainframe JCL is converted to Korn shell scripts, and Mainframe COBOL is */
/* converted to Micro Focus COBOL to run under Unix/Linux.                  */
/*                                                                          */
/* uvhd is especially valuable to investigate mainframe type files with     */
/* packed fields and no linefeeds (required by most unix/linux utilities)   */
/* uvhd can investigate all converted file types: sequential, fixed, variable,*/
/* indexed, text, binary, C-ISAM, D-ISAM, Micro Focus COBOL IDXFORMAT1,3,8) */
/*                                                                          */
/* ======================================================================== */
/* Owen Townsend, UV Software, 4667 Hoskins Rd, North Vancouver BC, V7K2R3  */
/*         Tel:   604-980-5434          Fax: 604-980-5404                   */
/*         Email: owen@uvsoftware.ca    Web: http://www.uvsoftware.ca       */
/* General purpose utilities for file conversion, maintenance,& sorting.    */
/* uvhd free program to investigate binary/packed files without linefeeds   */
/* ======================================================================== */

/*eject*/
/* uvhd is intended to be a 'FREE SAMPLE' of the Vancouver Utility package.*/
/*                                                                         */
/* See the web site for descriptions of other programs in the package      */
/* (The prices are available in the 'uvprices.doc' html document)          */
/*                                                                         */
/* uvhd is the 1 free program in the Vancouver utility package, which is   */
/* copy-righted and proprietary to UV Software Inc. You may copy & distribute*/ 
/* the uvhd program under the terms of the GPLv3 license (see above)       */
/*                                                                         */
/* See instructions to download & compile 4 pages ahead (about line# 220)  */
/*                                                                         */
char pid[72] = "uvhd.c Copyright: UV Software, 1993-2020, GNU/FSF GPLv3";

/*eject*/
/*--------------------- update history (latest 1st) -----------------------*/
/*                                                                         */
/*Mar11/2022 - comment out code to add 2 to scale for screen & print       */
/*           - no longer required after further changes to uvcopy z18      */
/*                                                                         */
/*Mar03/2022 - add option z16 RDW files hdrsize=total recsize vs data size */
/*           - getrdwh subtract hdrsize, existing logic assumes data size  */
/*           - write to add hdrsize, existing logic assuming hdr=datasize  */
/*           - write logic yet to do as of Mar03/2022 (version 20220311)   */
/*                                                                         */
/*Dec16/20 - add option t1,2,4,8 tchar replace unprintables on hex charline*/
/*         - t1 '.' (default), t2 ':', t4 '^', t8 '_'                      */
/*         - option 'l' line width default (LDFLT) changed from 64 to 100  */
/*                                                                         */
/*Mar21/19 - ignore t=string if not at least 2 hex chars, problem if t1,t2 */
/*Mar19/19 - default output record terminator to LF (option t2)            */
/*         - if variable lth (ftype != 'f') & no options 't#' or 'z' spcfd */
/*Mar19/19 - also print any stored opmsgs (as well as discplay on screen)  */
/*         - endscreen: copies opmsg1,2 to oppmsg1,2 for folwng printrec() */
/*Mar18/19 - test for data remaining at EOF for text file (when LF missing)*/
/*Mar18/19 - get ERRmsg on 1st rec (good), but rcount 1 less than actual   */
/*         - compensate by 1st time switch to add 1                        */
/*Feb26/19 - reset fileptr=0 if count 99 on w,u,m,n,g,e,a,t                */
/*Feb21/19 - record count 99 in parse sets count 999999999 process all recs*/
/*         - now if count 999999999 - also reset fileptr to BOF            */
/*         - testing 1st with v99                                          */
/*Feb19/19 - set scale-size = record-size if scalesize > recsize           */
/*May17/18 - fix STLr when pagesize 65K (Worldpay editterm.20180516.rel)   */
/*         - change max recsize to 20 bits vs 16 bits in switchx           */
/*Mar30/18 - update STL file handler for 5.1 system hdr records 32K vs 4K  */
/*         - STL 5.1 sysrecsize at 140(4) in hdr 32K/64K (vs 4K assumed)   */
/*Sep16/17 - clear vrtyp2 in getrecx if (upnext == 0) solves all problems  */
/*         - could remove other vrtyp2 clears (n/r in uvhdcob)             */
/*Sep15/17 - allow unused space > 4, up fileptr by actual vrsize unused    */
/*Sep12/17 - allow for unused space 1,2,3 (vs assuming 4)                  */
/*Aug28/17 - allow for unused space at end data STL Indexed/Relative files */
/*Dec06/16 - fix STL deleted record count (dont count deleted system recs) */
/*Nov09/16 - for moven: allow option 'x' multiple contiguous moves         */
/*Oct10/16 - for varlth +/- clear cargs before insert cargs.cmv & findrec()*/
/*         - problem for uvhd STLifile x4y7j4 show all but inhibit data recs*/
/*           for +1 just up recnum1++ & get next record                    */
/*           for +multiple: up recnum1 + multiple & use findrec()          */
/*Oct09/16 - add option y1 show hdr rec for STLS as well as STLI & STLR    */
/*         - allow for system record type x'10' as well as x'20'           */
/*Oct08/16 - add deleted count to displays & Index file save/load          */
/*Oct07/16 - add invalid R/T msgs for STL Indexed & Relative records       */
/*         - add deleted count for STL Indexed & Relative files            */
/*Oct06/16 - add STL options to show Deleted recs, filehdr,& system recs   */
/*         - force rebuild index if options changed                        */
/*Sep21/16 - for option v files & maxrec >4095, add w8 roundup multiple 8  */
/*Sep06/16 - fix options help screen g0 missing ',' concated g1 line       */

/*eject*/
/*Apr19/16 - fix bug STL recoord count 1 too high if Alternate Indices     */
/*         - change RSV files default v4 data records only (vs v5 hdr+data)*/
/*Apr15/16 - cannot assume Indexed file hdr always 2048+4096=6144          */
/*         - assume file hdr ID x'00' & use size from next 3 bytes         */
/*Mar03/16 - bypas Deleted records x'80' in STL Indexed & Relative files   */
/*Mar02/16 - change logic to read STL Indexed & Relative files             */
/*         - 1st byte hdr (vrtyp) x'40' = data, next 3 bytes slot size     */
/*         - 1st byte anything else = assume system record, size 4096 bytes*/
/*Feb26/16 - modify switchx 4 args, bits inhib +hdrsz now arg4 separate    */
/*         - switchb not used in uvhd, only switchx                        */
/*Feb20/16 - add support for STL files (sequential, Indexed,& Relative)    */
/*Sep11/15 - option d1 to inhibit null segments as well as blank segments  */
/*Aug22/15 - ensure default H64 defined unless H32 defined via -DH32       */
/*Jan16/15 - allow multi-byte record terminator in hex option t=xxxx       */
/*           uvhd filename t=FF01  <-- example for EBCDIC End-Of-Record    */
/*Dec24/14 - uvhd hard-coded table replaced by srcf/asc2ebc.c              */
/*         - after updates of Dec24/14 re [ ] & cent sign                  */
/*Nov02/14 - fix search1 allow highlight only vs search                    */
/*Sep02/14 - change test file read default to 9984 (vs 8000)               */
/*Jul02/14 - option p control action when both range & pattern spcfd       */
/*         - default p0 drop only if pattern match within range            */
/*                   p1 drop either wwithin range or by matching pattern   */
/*Apr23/14 - add option o# search for occurrence# of pattern within record */
/*         - change max lines/screen from 19 to 24 (default for option m)  */
/*Mar24/14 - allow search binary value --> s 65(4b),123456789 <-- example  */
/*         - remove command 'k' (expanded prompt), rely on ?=help          */
/*         - add command 'k' konvert among binary/packed/numeric/hex       */
/*Mar05/14 - clear the EOF msg at end drop cmd, emphasize HIGHLIGHT help   */
/*Mar04/14 - get filesize on close & compare to open filesize              */
/*         - warn if different (valid only if delete or trucnate           */
/*         - fileclose() added, openfile() changed to fileopen()           */
/*Feb23/14 - if op3 dsp(lth) omitted it defaults to op1 dsp(lth)           */ 
/*         - but if op4 constant lth not= op1 lth, deflt op3 entire record */
/*         - and same for op5 dsp(lth) omitted & op6 constant lth mismatch */
/*         - search without constant to highlight spcfd columns            */
/*Dec16/13 - for text files, default recsize 8000                          */
/*Oct24/13 - add option k4 newline after help prompt -->                   */
/*Oct21/13 - inhibit option d1 inhibit display blank segments if 1st segment*/
/*         - drop records requires option u                                 */
/*May20/13 - on update w hex constant, clear extra lth w nulls (vs blanks) */
/*Mar04/13 - fix h2 cmd not forcing hex display                            */
/*Feb22/13 - enhance drop, add options a/b c/d to delete by record# ranges */
/*Sep08/12 - default NO Index build for Text as well as Fixed records      */
/*Jul08/12 - if textfile default recsize to 512                            */
/*Dec02/11 - changes to compile with MinGW gcc (vs lcc) for Windows        */
/*         - #ifdef DWIN disable lockfile,sleep,                           */
/*         - enable LF64 always (MinGW allows, don't disable via !DWIN)    */
/*         - change lseeki64 to _lseeki64 for MinGW                        */
/*Nov11/11 - h0 hex(default), h1 force chars, h2 auto determine by data    */
/*         - add ts Translate zoned Sign from mainframe to Micro Focus     */
/*Oct08/11 - restore E64 printf %ld or %lld long or long long 32bit HP gcc */
/*Oct05/11 - make H64 the default (unless -DH32 spcfd)                     */
/*         - show machine & H32/H64 on last help screen                    */
/*         - remove hardcoded #define H64, for Mexico test -DH32 long long */
/*         - IBM Mexico compile problems on HP Itanium stat64 missing ?    */
/*         - move #includes prior to struct stat64 fstat1 (unlikely to fix)*/

/*eject*/
/***************************************************************************/
/*                      ** uvhd INSTALL/COMPILE **                         */
/***************************************************************************/
/*                                                                         */
/* For Unix/Linux you must download the source program and compile under   */
/* your Unix/Linux O/S. For Windows or DOS you may download the executable */
/* and store it in your program files or setup your environment PATH to    */
/* wherever you download.                                                  */
/*                                                                         */
/* - you must have downloaded the program from www.uvsoftware.ca           */
/* - may also download the custmas1 file to test/demo the program          */
/* - the following assumes the program has been stored in subdir 'src'     */
/* - you must ensure the filename is 'uvhd.c'                              */
/* - subdirectory 'bin' exists to receive the compiled output              */
/* - your Unix/Linux/Windows system has an ANSI C compiler                 */
/*                                                                         */
/* 1. cp uvhd.c src/uvhd.c   - store program in source subdir              */
/*    ====================   - ensure the filenmae is 'uvhd.c'             */
/*                                                                         */
/* 2. cc -DH64 src/uvhd.c -obin/uvhd  <-- compile the program              */
/*    ==============================      '-DH64' option for 64 bits       */
/*                                        '-DH32' option for 32 bits       */
/*                                                                         */
/* 3a. bin/uvhd dat1/custmas1 r256 - test with supplied downloaded datafile*/
/*    ============================                                         */
/*                                                                         */
/* 3b. bin/uvhd xxxxx r???   - display any of your data files (??? recsize)*/
/*     ===================                                                 */
/* 3c. bin/uvhd xxxxx t2     - use option 't2' for text (vs 'r' fixed rcsz)*/
/*     =================                                                   */
/*                                                                         */
/* 4. Add bin to your PATH so you can execute it from anywhere.            */
/*                                                                         */
/*    PATH=$PATH:/home/userx/bin; export PATH                              */
/*    =======================================                              */
/*                                                                         */

/*eject*/
/*----------------------- compile uvhd for Windows ------------------------*/
/*Note - no need to compile on Windows, since $UV/binDOS/uvhd.exe supplied */
/*     - but if desired, here is how I compiled the Windows version        */

/*Feb2003 - uvhd compiled on windows using lcc-win32 compiler in DOS box   */
/* D:\uvwin\uvhd - directory for windows compile, contains uvhd.c          */
/* PATH environment must include D:\lcc\bin PATH to lcc-win32 comiler      */
/* - must specify '-DDWIN' on lcc command line                             */

/* 1. lcc -DDWIN uvhd.c     - compile uvhd source to object                */
/* 2. lcclnk uvhd.obj       - link uvhd object to executable               */
/* 3. uvhd custmas1 r256    - execute/test uvhd on custmas1 demo file      */

/*                 ** Problems compiling on windows **                     */
/* - truncate function not handled by uvhd.exe (compiled by lcc-win32)     */
/* - see truncate function called by uvhd 'z' command (see help above)     */
/* - 'z' subfunction trunc1 calls the unix/linux truncate function         */
/* - truncate is defined by #include unistd.h on unix/linux                */
/* - bypassed (with errmsg) when compiled on windows by testing DWIN       */
/*   (compile time variable defined only on windows compiles)              */

/*------------------- compile on Windows with MinGW gcc -------------------*/
/* In Dec2011 I switched to MinGW (Minimalist GNU) to compile for Windows  */
/* See instructions at www.uvsoftware.ca/windowsdos.htm#G1 - G5            */
/* After MinGW installed, directories setup & VU files copied as page G1-G2*/

/*   mgcc1 uvhd H32   <-- compile uvhd for Windows                         */
/*   ==============                                                        */

/*eject*/
/***************************************************************************/
/*              ** possible customization re: PRINTING **                  */
/***************************************************************************/
/*                                                                         */
/* The print commands (p/i) use the command stored in variable 'prtcmd'    */
/*                                                                         */
/*     char prtcmd[30] = "uvlp12";     <-- default command                 */
/*                                                                         */
/* The 'uvlp12' script is part of the Vancouver Utilities package.         */
/* You can change this to the following & recompile as above.              */
/*                                                                         */
/*     char prtcmd[30] = "lp -onobanner -dlaser1";    (specify printer)    */
/*                                                                         */
/* Alternatively, you could add the following to your .profile             */
/*                                                                         */
/*     export UVHDPRINT="lp -onobanner -dlaser2"                           */
/*                                                                         */
/* Note that the other print command 'p' does not print immediately, but   */
/* collects all the printed data in a tmp/file, which is subsequently      */
/* printed manually after you quit the uvhd utility.                       */
/*                                                                         */
/* Mar2002 - added a 2nd variable for wide printouts                       */
/* Feb2003 - used only when option 'l' specified on 'p' command            */
/*                                                                         */
/*  char prtcmdW[30] = "uvlp12L"; <-- default cmd for print option 'i'     */
/*                                  - gets 100 chars on Landscape 8 1/2    */
/*                                                                         */
/*  export UVHDPWIDE="uvlp14L";   <-- env-var to modify default script     */
/*                                  - uvlp14L gets 128 chars on Landscape  */
/*                                                                         */
/*  export UVHDPWIDE="uvlp18";    <-- alternate for wide printouts         */
/*                                  - gets 128 chars on Portrait 8 1/2     */
/*                                                                         */
/*-------------------- uvhd files on distribution CD --------------------- */
/*                                                                         */
/* If you have installed the Vancouver Utiliites from CD (vs web download) */
/* the directories & filenames relevant to uvhd will be as follows:        */
/* (see complete details in install.doc)                                   */
/*                                                                         */
/* /home/uvadm/src/uvhd.c       - source program (ansi C)                  */
/* /home/uvadm/bin/uvhd         - executable program                       */
/* /home/uvadm/doc/uvhd.doc     - documentation as a text file             */
/* /home/uvadm/dochtml/uvhd.htm - documentation in HTML                    */

/*eject*/
/***************************************************************************/
/*                        ** uvhd functions **                             */
/***************************************************************************/
/*                                                                         */
/* 1. Browse  - display records consecutively                              */
/*            - jump to & display any record in the file by record#        */
/*                                                                         */
/* 2. Search  - search by pattern, anywhere in the record (by default)     */
/*              or within specified column locations                       */
/*            - may search on 1 or 2 fields with AND/OR relationships      */
/*            - match data =,>,< equal(default), greater than, less than   */
/*            - repeat previous search via 'ss' vs 's' + search args       */
/*                                                                         */
/* 3. Update  - update the current record on display                       */
/*            - specified columns replaced with specified data (char/hex)  */
/*            - option to rollback current record updates                  */
/*            - option for multi-record updates with 1 qualifier field     */
/*                                                                         */
/* 4. Print   - write formatted records to tmp file for later lp,uvlp,etc  */
/*            - output file will be: tmp/filename_yymmdd_hhmmssP           */
/*            - may specify a number of records from current position      */
/*            - may specify records matching 1 or 2 patterns & a max count */
/*            - To actually print, you must quit uvhd & select the desired */
/*              filename from the tmp directory.                           */
/*                                                                         */
/* 5. Iprint  - Immediate printing now option 'i' on the Print command     */
/*            - executes the 'uvlp12' script to send tmp/...P to printer   */
/*                                                                         */
/* 6. Write   - write UNformattted records to file: tmp/fname_yymmdd_hhmmssW*/
/*              for subsequent use as test files, etc.                     */
/*            - may specify number of records from current position        */
/*              or records matching 1 or 2 patterns within specified columns*/
/*            - select specified records from big files for small test files*/
/*                                                                         */
/* 7. Drop    - copy to tmp file, dropping records matching 1 or 2 patterns*/
/*            - Feb22/13 - add options a/b c/d to delete by record# ranges */
/*            - remove optn i drop DISAM deleted records (x'00' last byte) */
/*                                                                         */
/* 8. Check   - check sequence of specified field                          */
/*            - options for ascending, descending, duplicates              */
/*                                                                         */
/* 9. Tally   - count records from current position matching 1 or 2        */
/*              specified patterns until EOF or within a max record count  */
/*                                                                         */
/* 10. scan/replace - scan specified record field for a search pattern     */
/*                  - if found replace with specified replacement pattern  */
/*                                                                         */
/* 11. generate sequence numbers - command 'g'                             */

/*eject*/
/*--------------------------- help screens ----------------------------*/
/* - displayed if cmd line has no args or too many args                */
/* - also displayed by operator command '?'                            */

char *help01[30] = {
" ",
"                 ** H01. HELP menu, uvhd version 20220311 **",
" ",
"H01. Help screen menu, enter 2-24 for desired help screen",
"H02. command line options a-i",
"H03. command line options k-z",
"H04. Browsing, jump to byte# or rec# (fixlth), see 'H21' find varlth",
"H05. Search by data strings with optional qualifiers",
"H06. Search commands continued",
"H07. Update (u) & Rollback (x/X)",
"H08. Update      - more examples",
"H09. Enumerate/count(e) records (all or with specified data in fields)",
"H10. Accumulate(a) record field (zoned/packed/binary)(all or qualified)",
"H11. Write selected records to date/time stamped filename in tmp/ subdir",
"H12. Write options (a=ASCII,c=chars,r=recsize to write,t=terminator,etc)",
"H13. Print - writes to tmp/filename_date_timeP, option i print Immediate",
"H14. Translate to Ascii,Ebcdic,Upper,Lower,Characters,Periods",
"H15. sequence number checking & generating",
"H16. Scan/Replace (v) ",
"H17. Move character (m) & move Numeric (n), Example on 'H18'",
"H18. Move Example, reformat customer master into telephone list",
"H19. Konvert among numeric formats (zoned, packed, binary/hex)",
"H20. Drop records, writes backup to tmp/ in case restore required",
"H21. Truncate files & option t=multi-byte record terminators in hex",
"H22. find records by record# in large variable length files",
"H23. Index file saved in tmp/..., saves for next uvhd if same file&size",
"H24. uvhd Tips & Techniques (saving keystrokes)",
"H25. Customizing uvhd for User Preferences re Options & Printing",
"H26. uvhd version 20220311, Free Software Licensed under GNU FSF GPLv3",
"!"};

/*eject*/
char *help02[32] = {
" ",
"                   ** H02. command line options (a-i) **",
" ",
"uvhd dat1/custmas1 r256s2u  <-- Example (r256=recsize,s2=space2,u=updates)",
"==========================",
" ",
"- options entered as arg 2 on uvhd command line, must be contiguous string",
"- options are lower case alphabetics (optionally followed by numeric value)",
"a1  - translate to Ascii, character line (prior to zone & digit lines)",
"b#  - Begin display fileptr, used by uvhdcob exit to uvhd show same record",
"c1  - display extended ASCII Chars values 128-255 (vs showing as periods)",
"d1  - inhibit display blank/null 64 byte segments, except 1st in record",
"d2  - compress multi-blanks to 1, for Text files with large blank areas",
"d4  - show file Dsplcmnts on all segments vs only 1st segment of record",
"e1  - no Errmsg filesize/recsize not divisible, e2=no recsize calc, e3=both",
"f1  - build Index for Variable-Length files (BDW/RDW,IDXf8,text)",
"    - build Index for Fixed-Length files NOT required, can calc fileptrs",
"f0  - inhibit Index build (save time if only need to see 1st few records)",
"f3  - force Index re-build even if existing & filesize unchanged",
"g0  - zero relative column scale (default)",
"g1  - 1 relative dsplcmnts on displays & commands (search,update,etc)",
"h#  - h0=hex display (default), h1=force characters only, h2=auto determine",
"h2  - chars only or Hexadecimal (depending on data)",
"i1  - Inhibit bold search patterns, i2=no BOLD errmsgs, i3=both(DOS dflt)",
"l#   - Line display width, default 100, max 1024 ",
"m#   - Max lines/screen (default 24) allow for hex/char, sp1/sp2",
"n#   - Number of records per screen (between prompts, for short records)",
" ",
"!"};
/* "j1   - maintain command history .file (specify env-var UVHDROP)", */

/*eject*/
char *help03[30] = {
" ",
"              ** H03. command line options continued (k-z) **",
"o#   - inhibit column scale to every #records, use with n# records/screen",
"p#   - progress message interval (search,index) record count default 1000000",
"r#   - Record size, default 256, max 32768 ",
"s#   - Space before: s1=scale, s2=data, s4=prompt, s8=after scale, s15=all",
"t    - Text records ended by LF=x'0A' - standard unix/windows",
"t#   - replace unprintables on hex display t1=.(default),t2=:,t4=^,t8=_",
"t=FF01 - may specify multi-byte terminator in hex, example for EBCDIC EOR",
"       - see H21 example for EBCDIC EOR t=FF01",
"u    - allow Updates to file (will lock file) else read only",
"       applies to: Update,Move,V(scan/replace),Translate,Generate Seq#",
"u2   - allow Updates w/o file-lock (in case lock problems on some unixs)",
"v    - Variable length records Micro Focus Seqntl/Indexed IDXFORMAT3/8",
"v4     show recs: v1=filehdr, v2=deleted, v4=active data(default), v8=all",
"v_w8 - w8 round up Varlth recs to multiple 8 if recsize >4095 (use with v)",
"x    - STL records (Sequential, Indexed,or Relative)",
"x#     x2=Sequential, x4=Indexed, x8=Relative",
"x#y#   y4=show Deleteds, y1=show file Header, y2=show Systems, y7=show All",
"x#y#j4 j4=inhibit showing valid data records, y4j4=show Deleteds only",
"z#   - input RDW format (2,4,8 byte prefixes with recsize in binary)",
"z4   - RDW prefix 4 bytes, recsize 1st 2, 3&4 nulls, recsize INCLUDES prefix",
"z8   - BDW/RDW 4+4, BDW bypasswd, else like z4",
"z2   - RDW prefix 2 byte hdr recsize EXCLUDING prefix size",
"z18  - z2 RDW files with prefix hdr INCLUDING prefix size (z18=z2+z16)",
"z1   - (+ 1 bit) = z3/z5/z9 Little-end binary, z2/z4/z8 default BIG-end",
"?    - display help (20 screens)",
"!"};

char *help04[30] = {
" ",
"                   ** H04. file Browsing commands **",
" ",
" >>> uvhd commands begin here on 'H04' & continue thru 'H25' <<<",
"uvhd commands are entered at --> prompt following each record display",
" ",
"null - next record (or next segment if rcsz > 384)",
"r#   - goto specific record# in the file (one relative)",
"     - fast & accurate for fixed-record-length files",
"###   - may omit 'r', a number assumed to be a record# (if Fixed-length)",
"R#   - change Record size (vs option 'r' on command line)",
"b#   - goto specific  byte#  in the file (zero relative)",
" ",
"+#   - advance specified # of records (from current position)",
"-#   - backup  specified # of records (from current position)",
"+/-  - default 1 if no value specified",
" ",
"f#   - find records by record# in LARGE variable length files (RDW,text,IDX)",
"f#   - use 'f#' if you need exact record# in Large Variable-Length files",
"###  - may omit 'f', find record# assumed if BDW/RDW variable length file",
"     - see 'H21' for options to Index & find records by size min to max",
" ",
"!"};

/*eject*/
char *help05[30] = {
" ",
"                       ** H05. Search (1st of 2) **",
" ",
"uvhd dat1/custmas1 r256u - demo file shown at www.uvsoftware.ca/uvhd.htm#4A2",
"=======================  - option r256=recsize, option u if updates intended",
" - search for patterns until EOF (should goto rec #1 before 1st search)",
" - try these on the custmas1 demo file, then try similar on your file",
" - add 1 to following byte#s if you used option g1 for 1 relative (vs 0 rel)",
"s 'LTD'           - search for 'LTD' anywhere in record",
"ss                - repeat last search (starting from next record)",
"s 0(60),'LTD'     - search for 'LTD' anywhere in 1st 60 bytes",
"s 77(2),='BC'     - search for 'BC' in bytes 77-78 (match = assumed)",
"s 77(2),!'BC'     - search for Not 'BC' in bytes 77-78",
"s 120(5),x'000000000C'  - search for packed decimal zero field in 120-124",
"s 120(5),!x'000000000C' - search for non-zero packed field in 120-124",
"s 'ROAD',|,'STREET'     - search for ROAD or STREET anywhere in record (|=OR)",
"s 'ROAD',|,'STREET',|,'DRIVE' - search for 3 patterns anywhere in record",
"s 0(1),'1',77(2),'BC',90(3),'604'   - '1' in 0 And BC(77-78) And 604(90-92)",
"s 0(1),'1',|77(2),'BC',|90(3),'604' - '1' in 0 Or BC(77-78) Or 604(90-92)",
"s adrs1,value1,adrs2,value2,adrs3,value3 - 3 pairs of address & value",
" - 'AND' is the default when 2 or 3 conditions are specified",
" - 'OR' is signalled by the '|' (pipe symbol)",
"s 120(4b),338  - search for binary value (custmas1 rec#3 x'00000152' 120-123)",
"s 120(4bs),338 - need option 's' Switch ends for Big-end binary on Intel",
"!"
};

/*eject*/
char *help06[30] = {
" ",
"                      ** H06. Search (continued) **",
" ",
"uvhd dat1/custmas1 r256u - demo file shown at www.uvsoftware.ca/uvhd.htm#4A2",
"======================== - must specify option 'u' if updates intended",
" ",
"s  'LTD'          - search (forward by default) for LTD anywhere in record",
"ss                - repeat last search (starting from next record)",
"sss               - repeat search (in same record incrementing byte#)",
" ",
"s 77(2),'BC',242(4),>'1989',242(4),<'2000' - search for BC updated in 1990's",
" ",
"s 77(2),'AB',|77(2),'AL'   - search for 'AB' or 'AL in 77-78",
"s 77(2),'AB',|,'AL'        - same as above (2nd field defaults to 1st)",
"s 77(2),'AB',|,'AL',|,'YK' - search for 'AB' Or 'AL' Or 'YK' in 77-78",
"s 'LTD','ROAD','BC'        - searches entire record, all 3 must be found",
"s 'LTD',|,'ROAD',|,'BC'    - searches entire record, any 1 found is a match",
"s 77(2),'AL',,'LTD',,'ROAD' - search 'AL' in 77(2) & 'LTD' & 'ROAD' anywhere",
"  - if op3/op5 dsp(lth) omitted & op4/op6 constant lth not= op1/op2 length",
"    op3/op5 dsp(lth) defaults to search entire record for op4/op6 constant",
"s 77(2),'AB',,'AL',,'YK' - cant work, if op4/op6 constant lth = op1/op2",
"  - then op3/op5 dsp(lth) default same as op1 (AND condition in same field)",
"sn5 77(2),'BC'  - search for 5th occurrence of 'BC' in following records",
"so5 77(2),'BC'  - search for 5th occurrence of 'BC' in any 1 record following",
" ",
"s 77(2)  <-- HIGHLIGHT spcfd columns on all records (by omitting search data)",
"          - to identify desired fields & confirm col#s for subsequent search",
"s 77(2),,120(5),,235(5)  <-- may HIGHLIGHT up to 3 fields",
" ",
"!"
};

/*eject*/
char *help07[30] = {
" ",
"                      ** H07. Update (u) & Rollback (x/X) **",
" ",
"uvhd dat1/custmas1 r256u - demo file shown at www.uvsoftware.ca/uvhd.htm#4A2",
"======================== - must specify option 'u' when updates intended",
" ",
"u 77(2),'AB'    - update current record 77-78 with 'AB' (fixing some 'AL's)",
"x               - rollback last update (repeat 'x' toggles last update)",
"uu              - repeat last update (after moving to different record)",
" ",
"u99 77(2),'AB'  - update All records from 1st to EOF (99 short for 999999999)",
"                - count 99 resets to 1st record & processes until EOF",
"1               - u99 saves having to return to record#1 before update all",
"                - any other count does not reset (processes from current record)",
"                - no x/X rollback for multi-record updates (backup file 1st)",
" ",
"u99 77(2),'AB',77(2),'AL' - update next 99 recs with AB 77-78 if currently AL",
"u99 77(2),'AB',,'AL'   - same as above, 2nd adrs defaults to 1st",
"u99 9(1),'*',8(1),'D',77(2),'BC' - update byte 9 '*', if 8 is 'D' & prov BC",
" ",
"x/X - rollback last update or all updates to current rec ",
"x       - rollback last update to current rec (repeat x toggles last update)",
"X       - rollback ALL updates to current rec (repeat X toggles ALL updts)",
"x/X only for record on display & applies to both u(update) & v(scan/replace)",
"  - rollback applies only to 1 current record & ability lost if you move off",
"!"
};

/*eject*/
char *help08[30] = {
" ",
"                      ** H08. Update - more examples **",
" ",
"uvhd dat1/custmas1 r256u - demo file shown at www.uvsoftware.ca/uvhd.htm#4A2",
"======================== - must specify option 'u' if updates intended",
" ",
"u 75(1),'*',77(2),'BC' - test conditional update on 1 (current) record",
"uu99                   - repeat update on ALL records (99 short for 99999999)",
"                       - count 99 resets to 1st record & processes until EOF",
" ",
"              - OR for interactive updates, use 'ss' & 'uu' interleaved",
"s 77(2),'BC'  - 1st search to qualifying record",
"u 75(1),'*'   - 1st update to record on display",
" ",
"ss            - repeat last search (s 77(2),'BC')",
"uu            - repeat last update (u 75(1),'*')",
"              - can also use this for interleaved 'ss' & 'vv' (scan/rep)",
" ",
"u99s 77(2),'AB',,'AL' - update bytes 77-78 with 'AB' if currently 'AL'",
"   s        <-- note option 's' on 'u99s' saves update args in search args",
"1               for subsequent reset to BOF (by goto record #1)",
"ss          <-- and 'ss' to verify updates (without re-entering arguments)",
" ",
"!"
};

/*eject*/
char *help09[30] = {
" ",
"                           ** H09. Enumerate (e) **",
" ",
"uvhd dat1/custmas1 r256 - demofile#1 shown at www.uvsoftware.ca/uvhd.htm#4A2",
"=======================",
"e               <-- Enumerate/count all records from current position to EOF",
"e99             <-- count 99 resets to 1st record & processes until EOF",
"                  - any other count does not reset (processes from current record)",
"1                 - e99 saves having to return to record#1 before count all",
"e500 77(2),'BC' <-- count (in next 500 recs) recs with BC in bytes 77-78",
" ",
"e  77(2),'AB',|77(2),'AL'  - count records with AB or AL in bytes 77-78",
"e  77(2),'AB',|,'AL'       - same as above (2nd adrs defaults to 1st)",
"e  77(2),'BC',90(3),!'604' - count recs with BC in 77-78 & Not 604 in 90-92",
" ",
"e  'LTD'     - count records with 'LTD' anywhere in file, anywhere in record",
"             - scans all records in file   - when count omitted",
"             - scans all columns in record - when start(length) omitted",
"es 'LTD'     - 's' option save args for later 'ss' search to inspect 1 by 1",
" ",
"           ** lw/lp/lvw/lvp separate or combined write/print files **",
" ",
"lw  <-- may follow write command with 'lw' command to close file for separate",
"        output files or omit for combined outputs from multi write commands",
"lp  <-- similar command close print files for separate or omit for combined",
" ",
"!"
};

/*eject*/
char *help10[30] = {
" ",
"                           ** H10. Accumulate (a) **",
" ",
"uvhdcob dat1/custmas1 mf/maps/custmas1 r256 - customer mstr sales history",
"===========================================",
"                   - see custmas1 demofile at www.uvsoftware.ca/uvhd.htm#4A2",
"a 120(5p)        <-- accum 5 byte packed field from current record until EOF",
"=========",
"a99 120(5p)      <-- count 99 resets to 1st record & processes until EOF",
"                   - any other count does not reset (processes from current record)",
"1                  - a99 saves having to return to record#1 before acum all",
"a 120(5p),,77(2),'BC'              <-- acum if 'BC' in bytes 77-78",
"a 120(5p),,77(2),'AB',|77(2),'YK'  <-- acum if 'AB' or YuKon",
"a 120(5p),,77(2),'AB',|,'YK'       <-- acum if 'AB' or 'YK', omit 2nd 77(2)",
"a 120(5p),,77(2),'BC',90(3),!'604' <-- acum if 'AB' and area code not=604",
"ax12 120(5p)  <-- crossfoot & acum 12 x 5 byte packed fields (this yr sales)",
" ",
"uvhdcob dat1/sales2 mf/maps/sales2 r64 - sales detail records",
"======================================",
"             - see sales2 demofile at www.uvsoftware.ca/uvhd.htm#4A3",
"a 53(9)    <-- acum 9 bytes (assume unpacked Ascii, sign in zone or sep +/-)",
"=======      - zone +sign x'3_', -sign x'7_' (Micro Focus COBOL compatible)",
"a 53(9ze)  <-- acum 9 byte Zoned Ebcdic, +sign x'F_' or x'C_', -sign x'D_'",
"=========    - do not need (_ze) if option 'a' on cmd line for EBCDIC input",
"a 53(9zx)  <-- need (_zx) ASCII file with EBCDIC signs vs Micro Focus signs", 
" ",
"!"
};

/*eject*/
char *help11[30] = {
" ",
"               ** H11. Write selected records to tmp/ subdir **",
" ",
"w   <-- write unformatted data records to tmp/ (smaller test files etc)",
"      - writes start at current position & end at EOF (or max count)",
"      - outputs written/appended to $UVTMPDIR/filename_yymmdd_hhmmssW",
"      - $UVTMPDIR defaults to tmp/ in your current dir (not system /tmp)",
"      - writes fixedlth if input fixedlth, else defaults to LF terminated",
" ",
"w5              - write (unformatted) 5 records from current position",
"===",
"w99 'VANCOUVER' - write all records with 'VANCOUVER' anywhere in record",
"                - 'w99' is short for write ALL records from current to EOF",
"                - count 99 resets to 1st record & processes until EOF",
"1               - w99 saves having to return to record#1 before write all",
"w25 77(2),'BC'  - write (25 max) records with 'BC' in bytes 77-78",
"ww              - repeat last write command (from current position)",
"w 8(1),'D',77(2),'AB',|,'AL' - write recs with 'D' in 8 & AB or AL in 77-78",
" ",
"ws10            - select every 10th record (to make smaller test file ?)",
"w100e5          - write 100 records, select every 5th (file must have 500+)",
"ws10 77(2),'BC' - select every 10th rec with province 'BC' (from here to EOF)",
" ",
"w100r64t2 - sample Write command to Write next 100 records with options",
"          - r64=fixedsize 64 byte recs, t2=terminate with LF in last byte",
"w99z2a1c1 - write next 99 recs in RDWz2 format (recsize in 2 byte prefix)",
"          - translate to ASCII, translate any unprintable chars to periods",
"          - see write options on next help screen",
"!" };

char *help12[30] = {
" ",
"                       ** H12. Write options **",
" ",
"a1   - translate write command output records to ASCII",
"d#   - displacement to 1st byte to be written",
"e1   - translate write command output records to EBCDIC",
"c1   - convert unprintable chars in write output records to periods",
"n1   - insert variable recsize as 4 numerics at BEGIN write output",
"n2   - insert variable recsize as 4 numerics at END write output",
"r#   - record size for fixed-length output, omit for RDWz2/z4",
"t1   - append Carriage-Return at end write records (for MAC)",
"t2   - append Line-Feed at end of records (default if input file not fixedlth)",
"t3   - append both CR & LF at end write records (for some Windows programs)",
"t4   - append after Last-Non-Blank, else at end recsize (t6 for LF)",
"z#   - write RDW format (2 or 4 byte prefixes with recsize in binary)",
"z4   - prefix 4 bytes, recsize 1st 2, 3&4 nulls, recsize INCLUDES prefix",
"z2   - RDW prefix 2 byte hdr recsize EXCLUDING prefix size",
"z18  - z2 RDW files with prefix hdr INCLUDING prefix size (z18=z2+z16)",
"z1   - (add 1 bit) = z3/z5 Little-end binary, z2/z4 default BIG-end binary",
" ",
"w50d10r25t6 - Write next 50 records, option d10=dsplcmnt to 1st byte to write",
"            - r25=recsize to write, t6=terminate LF after last nonblank",
" - would write just customer name field of demo file dat1/custmas1",
"lw  <-- may follow write command with 'lw' command to close file for separate",
"        output files or omit for combined outputs from multi write commands",
"!" };
/*eject*/
char *help13[30] = {
" ",
"                ** H13. Print selected records to tmp/ subdir **",
" ",
"p  <-- print(write formatted records) to a file for subsequent lp,uvlp,vi,etc",
"     - writes $UVTMPDIR/filename_yymmdd_hhmmssP (concat for uvhd session)",
"     - $UVTMPDIR defaults to tmp/ in your current dir (not system /tmp)",
"     - export UVLPDEST=\"lp -dlaserxx\" for unix lp (see 'H24' below)",
"p5                - print 5 recs from current position",
"p25 'VANCOUVER'   - search & print (25 max) records with 'VANCOUVER' anywhere",
"p 77(2),'BC'      - search & print (til EOF) records with BC in bytes 77-78",
"p 77(2),'BC',90(3),!'604' - print recs with BC in 77-78 & Not 604 in 90-92",
"p 0(120),<x'20'     - print records with any bytes < space in 1st 120 bytes",
"p9s2f3            - print 9 records, space 2, formfeed every 3rd record",
"pp                - repeat last print command (from current position)",
"lp  <-- may follow print command with 'lp' command to close file for separate",
"        output files or omit for combined outputs from multi print commands",
" ",
"p5i <-- option 'i' Immediate print (Aug2011 change to option vs command 'i')",
"      - writes tmp/filename_yymmdd_hhmmssP & calls script uvlp12",
"      - script 'uvlp12' schedules 'lp' to print the tmp/... file immediately",
"        'export UVLPDEST=-dPRINTERNAME' in profile defines printer near you",
"      - see print control Environmental Variables on H24",
"      - see uvhd doc for more info on immediate printing",
"!"
};
/*eject*/
char *help14[30] = {
" ",
"       ** H14. Translate to Ascii,Ebcdic,Upper,Lower,Characters **",
" ",
"t  <-- translate to ASCII,EBCDIC,UPPER,lower,characters,periods",
"       ta=Ascii, te=Ebcdic, tu=Upper, tl=lower, tc=characters, tp=periods",
"ta         - translate to Ascii, entire current record, (t1a is same)",
"ta 0(120)  - translate to ASCII, 1st 120 bytes of current record",
"ta 240(16) - translate bytes 240-255 (zero relative, 241-256 one relative)",
"t99a    - translate to Ascii, ALL records in file (99 short for 999999999)",
"        - count 99 resets to 1st record & processes until EOF",
"1       - t99_ saves having to return to record#1 before translate all",
"te      - translate to Ebcdic, entire current record",
"ts      - translate zoned Sign from mainframe to Micro Focus conventions",
"          positives {ABCDEFGHI->0123456789, negatives }JKLMNOPQR->pqrstuvwxy",
"tu      - translate to Upper, entire current record",
"tl      - translate to Lower, entire current record",
"tc      - translate to Characters, any unprintables translated to blanks",
"        - unprintables are x'00'- x'1F' & x'7F'-x'FF'",
"tc1     - translate unprintable Chars to blanks, except LineFeeds x'0A'",
"tc2     - translate to Characters, except for Carriage-Returns x'0D'",
"tc4     - translate to Characters, except for FormFeeds x'0C'",
"tc8     - translate to Characters, except for Tabs x'09'",
"tc15    - translate to Chars, except LFs,CRs,FFs,Tabs (1+2+4+8=15)",
"tc3     - translate to Chars, except for LFs & Crs",
"tp      - translate any unprintable Characters to periods",
"tp15    - translate unprintables to periods, except LFs,CRs,FFs,Tabs ",
"t99p1 0(120) translate ALL records, unprintables to periods, except LF",
"!"
};
/*Apr04/06 - changed unprint char rep from period to carat */
/*         - did not like, should change back              */
/* - or provide 3 options tb=blanks, tc=carats, tp=periods */

/*eject*/
char *help15[30] = {
" ",
"              ** H15. sequence number checking & generating **",
" ",
"c        <-- check sequence in specified field - until EOF or SEQUENCE ERR",
"c  0(6)    - check sequence in key field 0-5 (cols 1-6), show 1st out of seq",
"cd 0(6)    - option 'd' to check sequence descending (vs ascending default)",
"ce 74(6)   - option 'e' to consider EQUAL keys out-of-sequence",
"ci 74(6)   - option 'i' to check sequence number increment by 1",
"ci10 74(6) - option 'i10' to check sequence number increment by 10, etc",
"cc         - repeat last sequence check (from current position to EOF)",
" ",
"g99 75(5),'00001' - generate seqnums in cols 76-80 incrmnt by 1 from 00001",
"                  - on all records from 1st to EOF (999 short for 999999999",
"                  - any other count does not reset (processes from current record)",
"                  - count 99 resets to 1st record & processes until EOF",
"g                 - if count omitted sequence# inserted only in current record",
"                  - must specify 'u' option on command line (like update)",
"g10 75(5),'00001' - generate seq# on next 10 records (vs g99 ALL records)",
"gg20              - repeat(gg) seq# on next 20 recs, same cols & inc as prior",
" ",
"gi10 72(8),'#00010.' - seqnum cols 74-79 from 000010 incrmnt by 10",
"       - any non-numeric data is inserted before & after the seqnum digits",
" ",
"g  74(6),'A00001',0(1),'A' - conditional seqnum, if col 1 = 'A'",
"r1                         - return to record #1 before next command",
"g  74(6),'B00001',0(1),'B' - conditional seqnum, if col 1 = 'B'",
"!"
};

/*eject*/
char *help16[30] = {
" ",
"                          ** H16. Search/Replace (v) **",
" ",
"v    <-- search/replace, scan for search string & replace with alternate",
"       - will truncate or blank fill if search/replace string lengths unequal",
" ",
"uvhd dat1/custmas1 r256u - demo file layout at www.uvsoftware.ca/uvhd.htm#4A2",
"======================== - must specify option 'u' if 'v' updates intended",
"v 10(25),'LTD','LIMITED' - replace 'LTD' with 'LIMITED' anywhere in 10-34",
"      - will lose any data pushed beyond byte 34 (fixed length record/fields)",
" ",
"v99 35(25),'RD.','ROAD'  - change RD. to ROAD til EOF (99 short for 999999999",
"v99 35(25),'RD.','ROAD',77(2),'BC' - replace only if BC in 77-78",
"                  - count 99 resets to 1st record & processes until EOF",
"                  - any other count does not reset (processes from current record)",
"1                 - saves having to return to record#1 before search/replace all",
" ",
"v99 'RD.','ROAD'         - a big mistake for fixed length record layouts",
"v99 0(256),'RD.','ROAD'  - same as this, entire record would be shifted",
" ",
"uvhd tf/tabtest1 t <-- demo TEXT file with tabs (unix expands to 8 spaces)",
"==================   - option 't' Text file, 'u' not req'd to write sep file",
"v99 x'09','    '  <-- replace tabs (x'09') with 4 spaces (tf/tabtest1 file)",
"                     - omitting dsp(lth) causes scan/rep entire record",
"!"
};

/*Nov25/10 - write option on scan/replace removed */
/*"v99w 35(25),'RD.','ROAD' - option 'w' to write separate file to tmp/...",*/
/*"   - writes tmp/filename_yymmdd_hhmmssV (Text records, this field only)",*/
/*"v99s 35(25),'RD.','ROAD' - option 's' save args for 'ss' search/verify", */
/*"   - records written to tmp/... will be expanded/contracted as required",*/

/*eject*/
char *help17[30] = {
" ",
"                   ** H17. Move character & move Numeric **",
" ",
"uvhd dat1/custmas1 r256u - demo file shown at www.uvsoftware.ca/uvhd.htm#4A2",
"======================== - must specify option 'u' when moves intended",
" ",
"m 35(12),90       <-- Move character (m), omit op2 lth assumes same as op1",
"m 35(30),90(12)   <-- move 90-101 to 35-46, 47-64 extra op1 length blanked",
" ",
"n 64(9z),120(5p)  <-- move Numeric (n), unpacks 5 bytes into 9 bytes Zoned",
"n 130(5p),64(9z)  <-- move Numeric (n), 9 zoned bytes to 5 packed bytes)",
"n 51(4b),120(5p)  <-- move Numeric, 5 bytes packed to 4 bytes binary",
"n 64(9ze),120(5p) <-- move Numeric, unpacks 5 bytes into 9 bytes Zoned EBCDIC",
"                  - data type codes z/p/b (z default) 1st alpha after length",
"                  - Ascii/Ebcdic codes a/e (a default) 2nd alpha after length",
"n 120(5p),123456  <-- move Numeric value (length omitted) to 5 bytes packed",
"nx12 120(5p),0      - use with option 'x' to clear multiple contiguous fields",
"n99x12 120(5p),0  <-- clear 12 mth sales in all records (99 means 999999999)",
"                    - n99 repeats moves on all records until EOF reached",
"                    - count 99 resets to 1st record & processes until EOF",
"n 120(5p),0,,77(2),='BC' <-- clear 5 bytes packed (move 0) if province 'BC'",
"                           - conditional moves via qualifiers in op3-op6",
"x             <-- use 'x/X' to rollback (only for moves on 1 record)",
"!"
};

/*eject*/
char *help18[30] = {
" ",
"                         ** H18. Move Example **",
" ",
"Move Example - create Telephone-List from custmas1 demo file (layout follows)",
"00-05 cust, 10-34 cust-name, 35-89 address, 90-91 tel#, 120-240 packed fields",
"desired: move tel# after cust-name, blank fill,& write 64 byte text records",
"             - see this example illustrated at www.uvsoftware.ca/uvhd.htm#4K1",
" ",
"cp dat1/custmas1 tmp/cm1  <-- 1st copy custmas1, rename (not final output)",
"========================",
" ",
"uvhd tmp/cm1 r256u        <-- run uvhd with Update option 'u'",
"==================",
" ",
"m99 35(30),90(12)         <-- Move tel# after cust-name & blank to 64",
"==================          - on all records n file (99 short for 999999999)",
" ",
"w99r64t6                  <-- Write, max Recsize 64, Textfile option 't6'",
"=========                   - t6 appends LF after last nonblank",
" ",
"cp tmp/cm1_yymmdd_hhmmssW dat1/tellist <-- copy/rename date_time_stamped",
"======================================     tmp file output as desired",
" ",
"!"
};

/*eject*/
char *help19[30] = {
" ",
"         ** H19. konvert numeric,packed,binary (display only) **",
" ",
"'k' converts Zoned,Packed,or Binary to Numeric or Hex (Displays Results)",
"  - results DISPLAYED ONLY, records NOT UPDATED, PROGRAMMER CONVENIENCE",
"kn - convert to Numeric display, k  - defaults to Numeric if n/h omitted",
"kh - convert to Hexadecimal display",
" ",
"uvhd dat1/testbnp1 r128 <-- file to demo binary,numeric,packed conversions",
"                 10        20        30        40        50        60   ",
"r#    1 0123456789012345678901234567890123456789012345678901234567890123",
"      0 0000001 0000256 0004096 0065536 1048576 .... .... .... .... ....",
"        3333333233333332333333323333333233333332000020000201002000020010",
"        0000001000002560000409600065536010485760100000100000000001000000",
"     64 ....... .....%l ......l ....eSl ....HWl .... .... .... .... ....",
"        0000001200000262000000620000656200014562000020000200102000020100",
"        000000C0000005C0000049C0000053C0000087C0000100010000000010000000",
"Binary 40(4b),45(4b),50(4b),55(4b),60(4b) are Little-End (Intel,x86 Solaris)",
"Binary 104(4b),109(4b),114(4b),119(4b),124(4b) Big-End (Sparc,AIX,HPUX,RISC)",
" ",
"k 40(4b) =1, k 45(4b) =256, k 50(4b) =4096, k55(4b) =65536, k60(4b) =1048576",
" ",
"k 40(4b)   --> 1          <-- x'01000000' on little-End machines (Intel, this doc)",
"k 40(4bs)  --> 16,777,216 <-- option 's' Switch ends, shows value on other end",
"k 104(4b)  --> 16,777,216 <-- x'00000001' on Big-End machines (AIX,SPARC,HPUX)",
"k 104(4bs) --> 1          <-- option 's' on Big-End shows little-end value",
"k x'00010000' --> 256 <-- may also enter a constant to convert (hex to numeric)",
"kh4 '256'     --> 00010000 <-- convert numeric constant to Hex 4 bytes",
"!"};

/*eject*/
char *help20[30] = {
" ",
"                     ** H20. Drop records **",
" ",
"drop - writes 3 date/time stamped files to $UVTMPDIR/ (default tmp/)",
"       tmp/filename_yyyymmdd_hhmmssB - Backup file (all records)",
"       tmp/filename_yyyymmdd_hhmmssC - Corrected file (after drops)",
"       tmp/filename_yyyymmdd_hhmmssD - Dropped records file",
" ",
"drop resets to record#1 to ensure entire file is backed up before drop",
"After 3 files copied to tmp/, 'C' file copied back overwriting original",
"- reopened as the new input file, & a prompt issued for next command",
"If no unique pattern exists, could update with unique flag in unused byte",
"- then return to record#1 & issue drop based on the unique flag",
" ",
"d 8(1),'D'              - copy to tmp/ dropping records with 'D' col 9",
"d 8(1),>'C',77(2),='BC' - drop records based on 2 patterns",
"da100b199               - drop records within range 100-199",
"da100b199c500d599       - drop records 100-199 & 500-599",
" ",
"options a/b & c/d will drop by record# ranges",
"option p  controls action when both range & pattern specified",
"       p0 - drop only if pattern match within range",
"       p1 - drop either wwithin range or by pattern match",
" ",
"da100b199 8(1),'D' - may combine drop by rec# ranges with drop by patterns",
"d500a100b199  - could limit no of records output (d500 would output max 500)",
"d99a100b199   - 'd99' means process to EOF (same as omitting number after d)",
" ",
"!" };

char *help21[32] = {
" ",
"                      ** H21. Truncate file **",
" ",
"z       - modify filesize (truncate or extend with x'00's)",
"z100000 - truncate file to 100,000 bytes",
"        - works on Unix,Linux,Windows/SFU, but not on Windows/DOS",
" ",
"              ** multi-byte hexadecimal record terminators **",
" ",
"uvhd dat1/testEORt1 at=FF01  <-- example EBCDIC End-Of-Record",
"===========================",
"                10        20        30        40        50        60   ",
"r#   1 0123456789012345678901234567890123456789012345678901234567890123",
"     0 dat1/testEORt1 - test uvcp typ=EORt1 translate to EBCDIC..",
"       88AF6A8AACDDAF464A8AA4AA894AA97CDDAF4A989A98A84A94CCCCCCF0",
"       413113523569310003523045370387E5693103915231350360523493F1",
"                10        20        30        40        50        60   ",
"r#   2 0123456789012345678901234567890123456789012345678901234567890123",
"    58 & append x'FF01' at EOR, depending rcs= on fili1/filo1     ..",
"       548998984A7CCFF748A4CDD64889898898498A749948898F68899F44444F0",
"       0017755407D6601D0130569B04575549570932E06506939116936100000F1",
" ",
"Note - uvhd options 'at=FF01'",
"     - option 'a' translates char line to ASCII (zones/digits lines EBCDIC)",
"     - option 't=FF01' specifies End-Of-Record ID for uvhd (new Jan 2015)",
"!" };

/*eject*/
char *help22[30] = {
" ",
"                       ** H22. find by record# **",
" ",
"f#  - find records by record# in LARGE variable length files (RDW,text,IDX)",
" r# - use 'r#' OK for Fixed-Length files, NOT for Variable-length",
"f#  - use 'f#' if you need exact record# in Large Variable-Length files",
"f#  - uses an Index of record#s & filepointers to find records quickly",
"    - Index built at program startup if variable lth input (RDW,text,IDX3/8)",
" ",
"### - can omit the 'f' or 'r' of record#, uvhd will determine the filepointer",
"    - based on the file type (calculate if fixed, use index if variable)",
" ", 
"                   ** option m & n to find records by size **",
" ",
"r1        - might return to record #1 before using options m & n",
"f99m123   - option 'm' to find record with specified record-size",
"f99m50n90 - option 'm' & 'n' to find records from minimum to maximum size",
"          - 'f99' used to search to EOF (99 is short for 999999999)",
"ff        - repeat prior find command, to find next rec with m & n size",
"ff        - 'ff' saves retyping big numbers & options m & n",
" ",
"!"};

char *help23[30] = {
" ",
"                  ** H23. Index file saved for re-use **",
" ",
"uvhd dat1/testBDWRDW z8a    <-- demo uvhd on BDW/RDW testfile",
"========================",
"vi tmp/uvhdindex_testBDWRDW <-- investigate saved Index file",
"===========================   - sample Index save file listed below",
" ",
"#01:uvhdindex_testBDWRDW",
"#02:filesize=999999,fileptr1=999900,options=z8",
"#03:IndexCreated=110428_110305,BuildTimeSeconds=0",
"#04:BDWmin=536,BDWmax=536,BDWavg=536,Blocks=2",
"#05:RCSZmin=28,RCSZmax=28,RCSZavg=28,Records=38",
"#06:1=0,2=32,3=60,4=88,5=116,6=144,7=172,8=200,9=228,10=256,11=284,12=312,",
"#07:13=340,14=368,15=396,16=424,17=452,18=480,19=508,20=540,21=568,22=596,",
"#08:23=624,24=652,25=680,26=708,27=736,28=764,29=792,30=820,31=848,32=876,",
"#09:33=904,34=932,35=960,36=988,37=1016,38=1044,38=1072,",
" ",
"- uvhd builds an Index file of file-pointers & record-numbers on startup",
"- saved to $UVTMPDIR/uvhdindex_...filename... to reload & save rebuilding",
"  the next time uvhd used on same filename with same filesize",
"- saved Index file contains statistics that may be useful to you",
"- contains 100 entries of rec#=fileptr (files < 100 records shows all rec#s)",
" ",
"!" };

/*eject*/
char *help24[30] = {
" ",
"                   ** H24. uvhd Tips & Techniques **",
" ",
" tip#1 - use command repeats to save keystrokes",
"       - use 'double letter commands' to repeat prior cmd w/o rekeying args",
"    ss - repeat last search, uu - repeat last update, mm - repeat last move",
"       - 1st re-position to desired rec before executing repeat command",
"       - or '1' to return to begin file to include all recs",
"   ww5 - may specify count on repeat commands, if original was 'w5 0(1),'A''",
"         ww5 would write next 5 recs with A in 1st byte (bypassing non 'A's)",
" ",
"tip#2 - Transfer Search arguments to other commands (w,e,p,i)",
"      - allows you to verify correct arguments before other commands:",
"      - then transfer verified args to desired command w/o having to re-key",
"        for example, we could specify args on search & transfer to write: ",
"  s 77(2),'BC',242(4),>'1997'  - search for BC records newer than 1997",
"  w=s                          - transfer Search args to Write command",
"  ww99                         - Write out selected records until EOF",
"      - transfer also applies to Enumerate(e=s), Print(p=s)",
" ",
"tip#3 - using NOT '!' condition, can use to find exceptions to most records",
"      - for example to find any provice codes that are NOT 'BC'",
"  s 77(2),!'BC'      <-- display records with province codes NOT = 'BC'",
"  w99 77(2),!'BC'    <-- write NON 'BC' records to tmp/filename_date_time",
" ",
"s 77(2)  <-- HIGHLIGHT spcfd columns on every record (by omitting search data)",
"!" };

char *help25[30] = {
" ",
"          ** H25. Tips & Customizing uvhd (User Preferences) **",
" ",
"tip#4 - '99' is short-code for 999999999 (IE - all records in file)",
"        w99  <-- Write All records until EOF",
"tip#5 - coding constant types for EBCDIC files vs ASCII default",
"      u 77(2),e'AB' <-- may code e'...' for Ebcdic vs c'...' for ASCII",
"      u 77(2),'AB'  <--BUT, may omit if option 'a' on command-line",
" ",
"Add Environmental Variables to .profile, saves options on command line",
"- for example setup option 'g1' if you prefer one relative to zero relative",
"export UVHDROP=g1   <-- user preference options example",
"                        g1=one relative byte#s (vs 0 rel default)",
" ",
"Environmental Variables for print option 'i' (Print Immediate)",
"export UVTMPDIR=tmp       <-- directory default for tmp files (print,write)",
"export UVLPDEST=-dlaserxx <-- specify printer destination for 'pi' command",
"export UVHDPRINT=uvlp12   <-- specify print script for 64 char lines",
"                            - uvlp12 is UV script for laser Portrait printing",
"export UVHDPWIDE=uvlp12L  <-- recommended script for Landscape 100+ chars ",
"                            - UVHDPWIDE used if you specify 'il100' ",
"export UVHDPRINT=\"lp -dlaserxx\" <-- may specify standard unix lp command",
"export UVHDPWIDE=\"lp -dlaserxx -onobanner\" - for 64 chars or 100 chars",
" ",
"!" };

/*eject*/
char *help26[30] = {
" ",
"                 ** H26. Program Licensed under GPLv3",
" ",
"'uvhd' is a binary file investigation utility. It displays any file in",
"vertical hexadecimal and prompts for commands to browse, search, select,",
"update, scan/replace, print, translate, etc. uvhd is an interactive",
"utility with a command line interface and 26 help screens.",
"See free download info at: www.uvsoftware.ca/freestuff.htm",
" ",
"                 ** Licensed under GNU FSF GPLv3 **",
" ",
"This program is free software: you may redistribute it and/or modify",
"it under the terms of GPLv3 (GNU General Public License version 3),",
"as published by the Free Software Foundation.",
"See the full description of the GNU General Public License at:",
"http://www.gnu.org/licenses.",
" ",
"This program is distributed in the hope that it will be useful,",
"but WITHOUT ANY WARRANTY; without even the implied warranty of",
"MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.",
" ",
"uvhd version 20220311, from Owen Townsend, UV Software, owen@uvsoftware.ca",
"!" };

/* dummy help27 */
char *help27[30] = {
" ",
"!" };

/*Jan10/11 - version date for screen/print headings */
char uvversion[16] = "20220311";

/*eject*/
/*uvhd----------------------- LargeFile support ------------------------*/
/*May2001 - test largefile -DLF64 set in compile script                 */
/*        - tested on unixware 7.1.1, see intro(2) describes 2 methods  */
/* method#1 _FILE_OFFSET_BITS=64   <-- excludes method#2 ??             */
/* method#2 _LARGEFILE_SOURCE 1  + explicit calls open64 lseek64        */

#define _LARGEFILE64_SOURCE 1
#define _FILE_OFFSET_BITS 64
#define __USE_LARGEFILE64 1

/*Dec08/07 - above used for other uv programs, check folwng Nov2004 cmt?*/
/*Nov2004 - RedHat Linux uses __USE_LARGEFILE64 ????? */
/*Dec2007 - adding __USE_LARGEFILE64 also ?? */
/*Dec08/07 - LF64 always defined for Linux (#if !defined(DWIN)) */
/*Dec02/11 - enable LF64 always (OK for MinGW, don't disable by !DWIN) */
#define LF64 1

/*uvhd-------------------- includes & defines ------------------------*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <malloc.h>
#include <errno.h>
#include <time.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <ctype.h>
#include <unistd.h>

/*eject*/
/*Dec07/07 - for win32 (lcc or gcc/MinGW), use _stati64 */
/*May18/08 - problems on Windows for 64 bit lseeki64  */
/* - tf/tabtest1 text files seek problem, reads 256 ? */
/* - stuck at EOF, r1 does not return to BOF          */
/* So - LF64 NOT defined on DWIN (see above)          */
/*    - use stat vs stati64 on DWIN (below)           */

/*Oct05/11 - IBM Mexico compile problems on HP Itanium stat64 missing */
/* - moved #includes prior to struct stat64 fstat1 (unlikely to fix)  */
/* BUT, then got stat64 undefined                                     */
/* - fixed by moving the LARGEFILE defines ahead of the #includes ?   */

#if defined(DWIN)
#if defined(LF64)
struct _stati64 fstat1;
char uvlargef[16] = "LF64W";
#else
struct stat fstat1;
char uvlargef[16] = "LF32";
#endif

#else  /* not DWIN */
struct stat64 fstat1;
char uvlargef[16] = "LF64";
#endif

/*eject*/
/* #include "UVtyp.h" - def's common to Vancouver utility C programs     */
/* - most Vancouver Utility prgms #include UVtyp.h header file           */
/*   but typdefs included in uvhd.c self-contained for internet freeware */

typedef int UVi32;
typedef unsigned char Uchar;
typedef unsigned int  intU;

/*Jul23/07 - chg compile script I64 to H64,S64,S32 (H64 to set PTR64) */
/*         - H64,S64 set I64 for 64 bit counters                      */
/*Dec05/07 - above chg in July was missed for uvhd until Dec05/07     */
/*Dec08/07 - H64/S64/H32 changed to L32/L64 + P64/P32                 */
/*Jan12/08 - compile scripts changed, L32/L64&P32/P64 combined to H32/H64*/
/* - always set I64 64 bit acums (long if H64, else long long assumed)*/
#define I64 1

/*Oct05/11 - make H64 the default (unless -DH32 spcfd) */
#if defined(H32)
char longbits[12] = "longlong64";
char H32H64[8] = "H32 ";
#else
#define H64 1
char longbits[8] = "long64";
char H32H64[8] = "H64 ";
#define PTR64 1
#endif

/* define UVi64: 64 bit long if -DH64, else long long     */
#if defined(H64)
typedef long UVi64;
#else
typedef long long UVi64;
#endif

#if defined(H64)
char E64[8] = "%ld";      /*Mar05/08 chg %lld to %ld */
#else
char E64[8] = "%lld";     /*Mar05/08 chg %d to %lld  */
#endif

/*eject*/
/* capture machine type from compile script manifest define       */
/* also define BEM (Big End Machines) vs LEM (Little End Machines)*/
#if defined(SCO)
char machine[8] = "SCO";
#elif defined(INT)
char machine[8] = "INT";
#elif defined(HP)
char machine[8] = "HP";
#define BEM 1
#elif defined(SUN)
char machine[8] = "SUN";
#define BEM 1
#elif defined(AIX)
char machine[8] = "AIX";
#define BEM 1
#elif defined(DEC)
char machine[8] = "DEC";
#define BEM 1
#elif defined(LNX)
char machine[8] = "LNX";
#elif defined(UWIN)           /* UWIN or CYGWIN under windows */
char machine[8] = "UWIN";
#elif defined(CWIN)           /* UWIN or CYGWIN under windows */
char machine[8] = "CWIN";
#elif defined(SFU)            /* SFU under windows */
char machine[8] = "SFU";
#else
char machine[8] = "XXX";
#endif

/* if BEM not defined, define LEM */
#if (!BEM)
#define LEM 1
#define BEM 0
int lem = 1;
int bem = 0;
#else
int lem = 0;
int bem = 1;
#endif
/*Mar24/14 - also define lem/bem 1/0 or 0/1 */

/*Jan2004 - replace compound machine type tests with 1 simple test   */
/* #if defined (INT) || defined (LNX)  <-- I sometimes forgot LNX    */

/*Dec07/10 - allow for EDEADLOCK not defined on some systems */
/*         - in /usr/include/sys/errno.h                     */
#ifndef EDEADLOCK
#define EDEADLOCK EDEADLK
#endif

/*eject*/
/*uvhd-------------------- defines,etc ------------------------------*/
#define RMAX  65536         /* maximum record size                   */
#define RDFLT 256           /* default rcsz - if no option 'r'       */
#define RDFLT2 1024         /* default rcsz - text optn & no optn 'r'*/
#define LMAX  1024          /* maximum line size                     */
#define LDFLT 100           /* default line size - if no option 'l'  */
/*Dec16/20 - LDFLT changed from 64 to 100                            */

time_t tmsecs;              /* unix time - seconds since 1970        */
char today[16];             /* todays date "yyyymmddhhmm"            */
char todttm[16];            /* todays date "yyyymm:ddhh"             */
char date6[8];              /* date in 6 digits                      */
char time6[8];              /* time in 6 digits                      */

/*------------------- DWIN/unix dependent defs ---------------------*/
#ifdef DWIN
char slashc = '\\';
char slashs[2] = "\\";
#else
char slashc = '/';
char slashs[2] = "/";
#endif

char cwdir[80];         /* current directory from $PWD, or null if undef  */


/*-------------------------- history file stuff --------------------------*/
/*Feb04/09 - most cmd history code removed (saved in src/uvhd_history.c)  */

char homedir[80];       /* $HOME directory or current dir if $HOME not def*/

char cwdir[80];         /* current directory from $PWD, or null if undef  */

/*eject*/
/*uvhd---------------------- filename & options -----------------------*/
/*Feb2003 - prompt for filename & options if not entered on cmd line   */
/*        - if reply null, then show help screens & quit as before     */

char fnops[256];            /* filename & options reply to prompt      */
char ops[64];               /* options separated from filename         */
int fnopsl;                 /* length of filename & option reply       */
int fnl;                    /* length of filename til ending blank/null*/
int opsl;                   /* length of options                       */
int flwait1;                /*Dec07/10 file lock wait acumulator       */
int lockstat;               /* setlock return status 0/1 not/locked    */

/*------------------------ option storage -----------------------------*/
/*Dec2001 - provide user preferences options env-var $UVHDROP          */
/*        - stored 1st, any cmnd line options appended                 */
char runops[80];            /* options from command line               */
char opsu[64];              /* options $UVHDROP + command line         */
char opsc[28];              /* option characters                       */
                            /* sorted into alpha position              */
int opsi[28];               /* option parameter integer storage        */
                            /* opsp numerics cnvrtd by atoi            */
char *opsp[28];             /* option string pointers                  */
char  opsb[256];            /* buffer for option string data           */


/*Feb2003 -  control codes to highlight found search patterns             */
char smso[16] = "\x1B\x5B\x37\x6D"; /* start bold standout mode           */
char rmso[16] = "\x1B\x5B\x6D";     /*  end  bold standout mode           */
int smsol;                          /* lth of smso calculated at prgm init*/
int rmsol;                          /* lth of rmso calculated at prgm init*/

/*Mar18/03 - warning message areas for printRV (reverse video)        */
char warnmsg1[100];
char warnmsg2[100];

/*Nov11/07 - filetype code f,t,v,x,z to simplify tests */
/* - set by options t,v,x,z, else default to f         */
char ftype = '?';

/*Dec16/20 - add option t1,2,4,8 tchar replace unprintables on hex charline*/
/*         - t1 '.' (default), t2 ':', t4 '^', t8 '_'                      */
char tchar;   /* replace unprints on hex t1=.(default),t2=:,t4=^,t8=_ */

/*eject*/
/*uvhd------------------- file access vital w/s ----------------------*/

char fn1[180];          /* filename from prmtr1 of cmnd line          */

char fn1a[180];         /* full path filename (for history file)      */

char fn1b[128];         /* base filename (after last '/')             */

/* May2001 - change fopen,fread,fseek,fwrite to open,read,lseek,write */
/*         - because there is no fseek64                              */
/* - will use: open64, lseek64, stat64 for Large Files (> 2 gig)      */
/* - will still use fopen,fwrite for output files (print,write,etc)   */

int fd1;                 /* file descriptor from open (May 2001)       */

char fmdate[16];         /* file modifiction date "yyyymmddhhmm"       */

int openflags;           /* fileopen flags default O_RDONLY (0)        */
                         /* changed to O_RDWR (2) by option u (update) */
                         /* see subfunc fileopen()                     */

char recb[RMAX+8];       /* record extracted from buffer               */

char *irecd = recb;      /* record data same as record base for uvhd   */
                         /* uvhdcob offsets by IDX or RDW prefixes     */
char *recd = recb;       /* record data same as record base for uvhd   */

char linr[LMAX+40];      /* record segment extracted from record       */
                         /* - for display on 3 vertical hex lines:     */

Uchar linc[LMAX+40];     /* line 1 - char with ctlchars cnvrtd to '.'s      */
Uchar linp[LMAX+40];     /* line 1 - smso/rmso codes inserted for search pat*/
char linz[LMAX+40];      /* line 2 - zones for vertical hex display         */
char lind[LMAX+40];      /* line 3 - digits for vertical hex display        */

/*eject*/
UVi64 filesize;          /* end of file fileptr (via stat)             */
UVi64 filesiz0;          /* filesize evenly divisible by rcsz (but <)  */
UVi64 filextra;          /* partial record at EOF (fsize % rsize)      */
int filextra2;           /* (fsize % rsize) for sprintf %d stop warning*/

UVi64 filesizeopn;       /* filesize on 1st open                       */
UVi64 filesizecls;       /* filesize on close, compared to open        */


UVi64 frecsc;            /* records calculated (filesize/rcsz)          */
char rcounts[16];        /* records= string from rec#05 on Index reload */
int rcount;              /* records for printing (screen headings)      */
int rcountv;             /* record count from findex variable lth files */
int rcountx;             /* set 1 at EOF in findrec() for varlth files  */
                         /* to inhibit calcrec for varlth files         */
int rcountf;             /* rec count in file ERRmsg LF missing last rec*/
                         /*Mar18/19 - 1st time switch to add 1          */

char dcounts1[16];       /* deleted= string from rec#05 on Index reload*/
int dcount1;             /* deleted count acum during Index create     */
char dcounts2[20];       /* edited deleted=9999 insert stats if dcount>0*/
int dcount2;             /* deleted count printing (screen headings)   */


int eof1;                /* count times reached EOF                    */
                         /* > 1 inhibits subsequent changes in rcount  */

int rmndr;               /* remainder filesize % recsize for errmsg    */
char rmndrs[32];         /* formatted into "fsize%%rsize(rmndr)=%d"    */

int rszo;                /* record size from option r or RDFLT         */
int rszox;               /* recsize option r/RDFLT +eXtra 256          */
int rszoy;               /* recsize option r/RDFLT +eXtra 128          */
int rcsmax;              /* recsize parse check max op for search,etc  */
                         /* rszo if Fixed, else rdwmax May28/11        */

int lszo;               /* line size from optn l or dflt to LDFLT      */
int lszh;               /* linesize = recsize if linesize > recsize    */

/*eject*/
/*---------------- current fileptr & recnum1 calc fields --------------*/

UVi64 fileptr;        /* new file position                             */
                      /* modified by user commands for byte# or rec#   */
                      /* never allowed < 0 or > filesize               */
UVi64 fileptrp;       /* fileptr from previous getrec()                */
UVi64 fileptr1;       /* ptr to last record in file fsz1-rszo (fixlth) */
                      /* stored when index built for varlth(RDW,text,IDX)*/
char fileptr1s[20];   /* used by findexload, store info from findexsave */
char fileops1[20];    /* used by findexload, force rebuild if runops chg*/
int IndexBuild;       /* set 1 for IndexBuild, allow x4y4j4 STL deletes only*/

UVi64 fpsv;           /* save file position                            */
int   rnsv;           /* save record number                            */
                      /* used to show last rec updated, written, etc   */

UVi64 recnum0;        /* current record# in file  (0 relative)         */
                      /* - calculated from fileptr on each get         */
                      /* - will subtract 1 when oprtr enters           */
                      /*   to calc new fileptr = (rec#-1 * rcsz)       */

int recnum1;          /* recnum1 for display/print (1 relative)        */
                      /* fixed size - recum + 1                        */
                      /* text/variable - increment on get              */
                      /*               - reset to 1 when fileptr reset */

int recnum1o;         /* recnum1 % option 'o' display scale every # recs*/

int rsz1;             /* current recsize - returned by getrec()        */

int rsz1p;            /*May25/10 Prior Record Size - saved at end getrec()*/
                      /* - used to incrmnt fileptr at begin next getrec*/
                      /* - for variable lth recs (not fixed)           */

int rsz2p;            /*Nov21/10 - recsize 2 recs back for -1 varlth   */

int upnext;           /*May25/10 - switch control fileptr update in getrec*/
                      /* - set off at begin prgm & by reposition r#/b#    */
                      /* - set on at end of getrec assuming browse follows*/

int rsz1d;            /* actual data size - depending on file type     */
                      /* (rsz1 - hdr size of RDW or IDX files)         */

int rsz2;             /* record size for current command calcs         */
                      /* - to calc prt from rec#                       */
                      /* fixed files - use rszo from r optn on cmd line*/
                      /* text files - use last rsz1                    */

/*eject*/
int lsz1;             /* current line size                             */

int rii;              /* record index to current byte in record        */
UVi64 rii1;           /* record index b4 current line segment          */
int lii;              /* line   index to current byte in line          */

int linctr;           /* line ctr on current screen                    */
                      /* depending on hex, char only, space 1/2        */

int linmax;           /* max line count per screen option m            */

int xchar;            /* indicator 0/1 print current record in hex     */
                      /* 1 if any < 0x20 or > 0x7E                     */
                      /* But allow hex chars in last 2 bytes of record */
                      /* - to allow LF in last byte or CR/LF last2     */
                      /* also depends on option 'h'                    */
                      /* h0=auto determine, h1=chars only, h2=force hex*/

char firstop[12];     /* option f# for first rec to be displayed       */

int rpsctr;           /* records per screen - for option n              */
                      /* used only on null reply when browsing          */
UVi64 fileptro1;      /* fileptr saved from 1st on screen               */
                      /* restored on cmds other than browse, rec#, byte#*/

char opmsg1[128];     /* operator msg1, stored by subrtns       */
char opmsg2[128];     /* operator msg2, stored by subrtns       */
char opmsg3[128];     /* operator msg3, stored by subrtns       */
                      /* for display after current screen       */
char oppmsg1[128];    /* printrec msg1, for print after display */
char oppmsg2[128];    /* printrec msg2, for print after display */
char oppmsg3[128];    /* printrec msg2, for print after display */
/*Mar19/19 - also print any stored opmsgs (as well as discplay on screen)  */
/*     - endscreen: copies opmsg1,2,3 to oppmsg1,2,3 for folwng printrec() */

/*May28/11 - search progress message interval option p default 1 million */
int pmsgcnt;          /* option p default 1000K (1 million) */
int pmsgctr;          /* counter up to option p for msg     */

/*Jan16/15 - allow multi-byte record terminator in hex t=xxxx */
/* uvhd filename t=FF01  <-- example for EBCDIC End-Of-Record */

char eoridc[16];      /* t=cccc hex characters */
char eoridx[8];       /* t=xx converted to true hex */
int eorlc;            /* hex char string lth */
int eorlx;            /* true hex string lth */
int eormi;            /* match index in eoridx[8] */

/*eject*/
/*uvhd---------------------- getcbuf buffer ----------------------------*/
/* for getcbuf subfunction - get 1 byte at a time from a text file      */
/* - returns 1 character (unsigned) or EOF (-1) at end of file          */
/* - buffered by reading rszo (r option) bytes at begin each getrec     */
/* - or if buffer becomes empty before LF reached                       */

char bufa[16384];        /* buffer read area            */
char *bufp = bufa;       /* buffer pointer to next char */
int bufn = 0;            /* buffer bytes remaining      */


/*---------------------------- work area -------------------------------*/
/* work area for scan/replace - copy out & back to record field         */
char wa1[RMAX+8];        /* work area for scan/replace                  */


/*----------------------- help screen pointer table --------------------*/
/* for showhelps function (allows user to enter desired screen#)        */

char **hsp[27] = {
help01,
help02,
help03,
help04,
help05,
help06,
help07,
help08,
help09,
help10,
help11,
help12,
help13,
help14,
help15,
help16,
help17,
help18,
help19,
help20,
help21,
help22,
help23,
help24,
help25,
help26,
help27
};

/*May27/10 - switch k0 to kill help prompts & k1 to restore */
/*Mar24/14 - remove expanded prompt & command 'k', rely on ?=help */
int helpson = 0;        /* help prompts initially off */

/*eject*/
/*------------------------ column scale --------------------------------*/
/* work areas used to setup column scales for units & tens              */
/* depending on option g0/g1 for zero/one relative                      */

char scaleu0[12];        /* '0123456789' or '1234567890'  */
char scalet0[12];        /* '________10' --> '______1030' */

char scaleu1[1032];      /* units scale built depending on option g0/g1 */
char scalet1[1032];      /* tens scale built depending on option g0/g1  */

char scaleu2[1032];      /* units scale copied & truncated depending on lsize*/
char scalet2[1032];      /* tens scale copied & truncated depending on lsize*/


/*Oct08/11 - restore E64 printf %ld or %lld long/longlong 32bit HP gcc   */
char fpedit1[20];        /* fileptr edited by E64 %ld/%lld long/longlong */
char fpedit2[20];        /* fileptr or record dsplcmnt on 2nd+ segments  */
char fsedit[20];         /* filesize edited by E64 %ld/%lld long/longlong*/
char fpedit1e[20];       /* fileptr to last record in file %ld/%lld      */

char blank256[256];      /* initialized to all blanks                  */
char null256[256];       /* initialized to all nulls (by default)      */

/*eject*/
/*uvhd-------------------- command formats -----------------------------*/
/* command format - 2 types                                             */
/* type1 - 1 char command + optional numeric value                      */
/* r100                     - goto & display record #100                */
/*                                                                      */
/* type2 - cmd + op1,op2,op3,op4,op5,op6                                */
/* s 0(20),='ABC'             - search for ABC within 1st 20 bytes      */
/* p 0(3),>'800'              - print records > 800 in 1st 3 bytes      */
/* u 0(3),='ABC'              - update cols 1-3 of current rec w ABC    */
/* s ='ABC'                   - search for ABC anywhere in entire record*/
/* s 0(3),!'ABC'              - search for NOT ABC in cols 1-3          */
/*note - op1 search lth must match op2 constant lth                     */
/*       if searching NOT EQUAL (!) on fields > 1 byte                  */
/*                                                                      */
/* s 0(40),='ABC',40(40),='XYZ' - search for ABC in 1-40 & XYZ in 41-80 */
/*                                                                      */
/* s 0(1),='1',40(1),='A'   - search for 1 in byte 0 AND A in byte 40   */
/* s 0(1),'1',|40(1),'A'    - search for 1 in byte 0 OR A in byte 40    */
/*                                                                      */
/* s 0(1),>'1',40(1),<'Z'   - search for >1 in byte 0 & <Z in byte 40   */
/*                                                                      */
/* s 0(80),<x'20',0(80),>x'7E' - search for non printable chars in 1-80 */
/*                                                                      */
/* s 0(1),'1',|,'2'         - op3 omitted defaults to op1 search area   */
/*                          - must specify OR (|) for = conditions      */
/* s 0(1),>'1',,<'9'        - search for 2-8 in byte 0 (range search)   */
/*                                                                      */
/*eject*/
/*uvhd----------------- command argument storage -----------------------*/
/*                                                                      */

char opcmd[80];                /* oprtr reply to command prompt         */
char opcmd2[80];               /* op reply alt w/s to dflt 99 to r99    */

char cms[80];                  /* command string (up to 1st space)      */

char cops[80];                 /* command options string                */

char copsn[24];                /* numeric digits isolated from cops     */

UVi64 cmn;                     /* numeric value appended to cmnd        */
                               /* May2001 - cmn changed to UVi64        */

char copsc[28];                /* cmd options char array                */
int copsi[28];                 /* cmd options integer array             */

char args[128];                /* arguments (after 1st space to next sp)*/
int argsd;                     /* dsplcmnt to current arg (for sdscopy) */

/*Dec07/10 - Ascii or Ebcdic Blank if optn 'a', for move/updt clear excess*/
char AEB = ' ';          /* changed to 0x40 if option a EBCDIC data input */
char AEC[4] = "=c";      /* changed to "=e" if option a EBCDIC data input */

/*eject*/
/* args separated op1,op2,op3,op4 before standardizing & defaulting     */
char arg1a[80];                /* field1 search area - dspl(lth)        */
char arg2a[80];                /* field1 search constant                */
char arg3a[80];                /* field2 search area - dspl(lth)        */
char arg4a[80];                /* field2 search constant                */
char arg5a[80];                /* field3 search area - dspl(lth)        */
char arg6a[80];                /* field3 search constant                */

/* args after defaulting & standardizing constants =c'ccc' >x'xx',etc   */
char arg1b[80];
char arg2b[80];
char arg3b[80];
char arg4b[80];
char arg5b[80];
char arg6b[80];

/* args after stripping quotes/conditions & cnvrsn to hex if indicated  */
char arg1c[80];
char arg2c[80];
char arg3c[80];
char arg4c[80];
char arg5c[80];
char arg6c[80];

/* arg dsplcmnts & lengths (only lengths for constants op2/op4)       */
int arg1d;                    /* op1 displacement                     */
int arg1l;                    /* op1 length                           */
int arg2d;                    /* op2 dsplcmnt for move Dec06/10       */
int arg2l;                    /* op2 length (constant)                */
int arg3d;                    /* op3 displacement                     */
int arg3l;                    /* op3 length                           */
int arg4l;                    /* op4 length (constant)                */
int arg5d;                    /* op5 displacement                     */
int arg5l;                    /* op5 length                           */
int arg6l;                    /* op6 length (constant)                */
int op1dflt;                  /* op1 defaulted to 0(rsz1)             */

/*Feb23/14 - indicators for arg3 & arg5 omitted (see parse mods)      */
int arg1x;   /* arg1 omitted dflt entire record, shift constant to op2*/
int arg3x;   /* arg3 omitted dflt entire record if arg4 lth mismatch  */
int arg5x;

/*Feb23/14 - dsplcmnt & lenth to higlight (search w/o op2 contant) */
int hil1d;
int hil1l;
int hil2d;
int hil2l;
int hil3d;
int hil3l;

/*eject*/
/*uvhd----------------- command structure storage ----------------------*/
/* structure to store command values                                    */
/* - initially stored here, then moved depending on 's' 'p' 'u'         */

struct CA {
  char cmd[80];             /* orig cmnd for errmsgs             */
  char cmc[80];             /* command chars                     */
  char cmo[80];             /* command options                   */
  char a2c[80];             /* op2 constant data                 */
  char a4c[80];             /* op4 constant data                 */
  char a6c[80];             /* op4 constant data                 */
  char a1cc[4];             /* op1 condition (not used)          */
  char a2cc[4];             /* op2 condition <=!> (in 1st byte)  */
  char a3cc[4];             /* op3 condition '|' 1st byte = OR   */
  char a4cc[4];             /* op4 condition <=!> (in 1st byte)  */
  char a5cc[4];             /* op5 condition '|' 1st byte = OR   */
  char a6cc[4];             /* op6 condition <=!> (in 1st byte)  */
  int cmv;                /* cmd value (numerics appended)     */
  int a1d;                /* op1 dsplcmnt                      */
  int a1l;                /* op1 length                        */
  int a2d;                /* op2 dsplcmnt for move Dec06/10    */
  int a2l;                /* op2 length                        */
  int a3d;                /* op3 dsplcmnt                      */
  int a3l;                /* op3 length                        */
  int a4l;                /* op4 length                        */
  int a5d;                /* op5 dsplcmnt                      */
  int a5l;                /* op5 length                        */
  int a6l;                /* op6 length                        */
  int op1dflt;            /* op1 defaulted to 0(rsz1)          */
  int pscd;               /* prior search continuation dsp     */
  int psfp;               /* prior search file ptr             */
  int opi[26];            /* cmnd options - integer            */
  char opc[26];           /* cmnd options - character          */
  char a1dt;              /* Data Type - Packed/Zoned/Binary   */
  char a1dc;              /* Data Code - Ascii/ABCDIC          */
  char a1sgn;             /* Data Sign to insert +/-           */
  char a2dt;              /* Data Type - Packed/Zoned/Binary   */
  char a2dc;              /* Data Code - Ascii/ABCDIC          */
  char a2sgn;             /* Data Sign to insert +/-           */
  /*Dec26/10 - a2dt,a2dc,a2sgn added for move numeric          */
};

/* note re pscd - op1 dsp prior match (for search continue)      */
/* cmd ss  - repeat search (from next record)                    */
/* cmd sss - repeat search (same record, incrmnt column)         */
/* cmd ssr - repeat search (same record, resetting to column 1)  */

/*eject*/
/* command structures - saved for repeats */
struct CA carg0;            /* cmd arg structure for clearing    */
struct CA cargs;            /* cmd args processing storage       */
struct CA sargs;            /* search args strorage              */
struct CA pargs;            /* print args storage                */
struct CA iargs;            /* print args storage                */
struct CA wargs;            /* write args storage                */
struct CA uargs;            /* update args storage               */
struct CA dargs;            /* copy/drop args storage            */
struct CA eargs;            /* enumerate args storage            */
struct CA vargs;            /* scan/replace args storage         */
struct CA targs;            /* translate args storage            */
struct CA gargs;            /* generate sequence number          */
struct CA fargs;            /* find record# in varlth files      */
struct CA margs;            /* move field/constant               */
struct CA nargs;            /* move field/constant               */
struct CA aargs;            /* acumulate record field            */
struct CA xargs;            /* for acum crossfoot Feb26/11       */
struct CA zargs;            /* check seq chgd from kargs Mar24/14*/
struct CA kargs;            /* konvert among binary/packed/num/hex*/

/*eject*/
/*uvhd----------------------- output files -----------------------------*/
/*Feb05/09 - common functions openfileX & closefileX for P,I,W,V,D files*/
/* - chg P/I/W/V/D cmds to open & close output file on each cmd */
/* - cmd line option w1 to write 1 combined file REMOVED 2010   */
/* - filename format as follows:                                */

/* tmp/inputfilename_yymmdd_hhmmssX                     <-- X=P/I/W/V/D */
/* tmp/inputfilename_yymmdd_       <-- basefilename + time + X each open*/
/*May03/11 - provide $UVTMPDIR define subdir for print,write,drop,index,etc*/

char basename[128];            /* basefilename formatted at prgm init  */

/*------------------ output file for print command --------------------*/
/* print filename = $UVTMPDIR/inputfilename_yymmdd_hhmmssP             */

char pfname[128];              /* print output filename built here */
FILE *pfptr;                   /* output file for print command    */
int pfopn;                     /* switch to open file on 1st print */
int pfctr;                     /* print output counter             */

/*------------------ output file for iprint command ----------------*/
/* iprint filename = $UVTMPDIR/inputfilename_yymmdd_hhmmssI         */
/*Aug15/11 - iprint changed to option 'i' on print command          */

char ifname[128];              /* iprint output filename built here */
FILE *ifptr;                   /* output file for iprint command    */
int ifopn;                     /* switch to open file on 1st print  */
int ifctr;                     /* iprint output counter             */

/* Iprint commands - modified depending on unix or DWIN               */
/*   - & depending on env-vars UVHDPRINT & UVHDPWIDE                  */
/*   - or you could modify source code here to 'lp' if desired        */
#ifdef DWIN
char prtcmd[30] = "uvlpr12";   /* iprint cmd string for system cmd    */
char prtcmdW[30] = "uvlpr12L"; /* iprint cmd Wide (Landscape?) i#l100 */
#else
char prtcmd[30] = "uvlp12";    /* iprint cmd string for system cmd    */
char prtcmdW[30] = "uvlp12L";  /* iprint cmd Wide (Landscape?) i#l100 */
#endif

char prtcmdfile[128];          /* iprint cmd string w file appended   */
char prtcmdWfile[128];         /* iprint Wide cmd with file appended  */

/*eject*/
/*uvhd-------------- output file for write command --------------------*/
/* write filename = $UVTMPDIR/inputfilename.datetimeW                  */
char wfname[128];              /* write output filename built here     */
FILE *wfptr;                   /* output file for write command        */
int wfopn;                     /* switch to open file on 1st write     */
int wfctr;                     /* write output counter                 */

char wrecb[RMAX+8];         /* record area for write command        */

char *wrecd = wrecb;        /* adrs for data, offset by optns z2,z4,n1 */

int wrhs;              /* record hdr size for Write records            */

int wrs1w;             /* recsize for write I/O, for RDWz2 = data + 2  */
                       /* for RDWz4 = data + 4                         */

int wrs1h;             /* recsize for RDW hdr, sames as wrs1w for RDWz2*/
                       /* - must add 4 for RDWz4                       */
short wrs1hs;          /* write size in short for union switch ends    */

int wrsn1;             /* length of recsize numerics inserted by n1 */
                       /* - set to 4 if n1, else 0                  */
int wrsn2;             /* length of recsize numerics inserted by n2 */
                       /* - set to 4 if k2, else 0                  */
int wrsn3;             /* wrsn1 + wrsn2 (4 or 0)                    */

char *wrecn1;          /* adrs for optn n1 numeric recsize insert at bgnrec*/
char *wrecn2;          /* adrs for optn n2 numeric recsize insert at endrec*/

/*Mar03/2022 - add option z16 RDW files hdrsize=total recsize vs datasize     */
/* z16 - subtract hdrsize in getrdwh, existing logic assumes hdrsize=datasize */
/*     - write to add hdrsize, existing logic assuming hdr=datasize           */
/*     - write logic yet to do as of Mar03/2022                               */

/*eject*/
/*------------------ output file for scan/replace command ----------------*/
/*May16/08 - scan/replace command added                                   */
/* $UVTMPDIR/inputfilename_yymmdd_hhmmssV <-- scan replace filename format*/

char vfname[128];        /* filename_yymmdd_ formatted at program init    */
FILE *vfptr;             /* output file pointer for scan/replace command  */
int vfopn;               /* switch to open file on 1st scan/replace       */

/*------------------ output file for drop command ------------------*/
/*Feb24/11 - add 2nd file for Drop command to write dropped records */
/*Apr05/11 - new version of 'drop' command - copies 3 files to tmp/ */
/* - tmp/fname_yyyymmdd_hhmmssB   <-- Backup file (all records)     */
/* - tmp/filename_yymmdd_hhmmssC  <-- Correct (not-Dropped) records */
/* - tmp/filename_yymmdd_hhmmssD  <-- Dropped records               */

char bfname[128];              /* Backup filename for delete command*/
FILE *bfptr;                   /* output file for Backup records  */
int bfopn;                     /* switch to open file on 1st drop */

char cfname[128];              /* Correct (not-dropped) filename  */
FILE *cfptr;                   /* output file for Correct records */
int cfopn;                     /* switch to open file on 1st drop */

char dfname[128];              /* Dropped records filename        */
FILE *dfptr;                   /* output file for Dropped records */
int dfopn;                     /* switch to open file on 1st drop */

/*------------------- file status structure ---------------------------*/
/* - to test for presence of 'tmp' sub-directory                       */
/* - will create if not present in current directory                   */
/*May03/11 - add $UVTMPDIR define subdir for print,write,drop,index,etc*/

struct stat tmpstat;           /* file stat structure - test tmp dir   */
char tmpdir[128];              /* $UVTMPDIR or tmp                     */
char mkdirtmp[100];            /* system("mkdir $UVTMPDIR");           */

/*eject*/
/*--------------------------- update storage ---------------------------*/
/* work storage for updates - sample update commands follow             */
/*   u0(3),='ABC'    - change 1st 3 bytes of current record to 'ABC'    */
/*   u10(3),=x'0D0A' - change bytes 11 & 12 of current record to x'0D0A'*/

char recsav1[RMAX+8];            /* current record before any updates   */
char recsav2[RMAX+8];            /* current record before last update   */

UVi64 fplastup = -1;             /* fileptr of last record updated      */
                                 /* -1 for init purposes                */

UVi64 seekstat;                 /* I/O status for seeks                 */

int writestat;                  /* I/O status for writes                */

UVi64 statstat;                 /* I/O status for stat                  */

int opstat;                     /* status returned by update & rollback*/

/*--------------------- check sequence field storage --------------------*/

char chksav1[128];               /* check sequence field storage         */
char chksav2[128];               /* check sequence field storage         */

int chksav1i;                    /* for option 'i' incrmnt i1,i10,etc    */
int chksav2i;

/*eject*/
/*uvhd---------------- MF COBOL variable lth records ------------------*/
/* added 9911, option v (existing option v changed to option t)        */
/* - folowing aalso used for STL files                                 */

char vfhdr[8192];      /* MF COBOL variable lth file hdr stored here   */
                       /*Dec2004 - increased to 8K for IDXFORMAT8      */
int vrmax;             /* max rcsz from file hdr bytes 54-57           */
int vrmin;             /* min rcsz from file hdr bytes 58-61           */

int vrsys;             /* STL system record size from file hdr 140(4)  */

int vfhs;              /* file header size for IDXFORMAT8              */
int vfxtra;            /* vfhs-128 to read remainder of file header    */
int vrhs = 0;          /* rec hdr size = 4 if maxrcsz > 4095, else 2   */
int vrup48 = 0;        /* switchx/b roundup multiple 0/4/8             */

union INTC             /* union used in getrecv                        */
{ int ii;
  short ss;
  char cc[4];
};

int vrsize;            /* current record size                         */
                       /* extracted from the 2 or 4 byte hdr          */
int vrslot;            /* current record slot (hdr+data+fill to mult4)*/

Uchar vrtyp;           /* current record type                          */
                       /* isolated from hdr zone only, digit cleared   */
Uchar vrtyp2;          /* vrtyp saved from prior readhdr/switchx       */
                       /* to test for unused space at end data records */
Uchar vrextra;         /*Sep15/17 - calc when unused space > 4         */

/*Aug28/17 - vrtyp2 from prior readhdr/switchx for unused space at end data*/
/*           vrtyp2 & 0x08 = unused space at end data record exists        */
/*                           unused space value is in 4 bytes at end data  */

int vfhs = 128;        /* file hdr size (128 bytes)                   */

/*Pre Nov2002 - vfhs used to calc fileptr from rec#, +recs, -recs       */
/*            - presuming Indexed files, no good for RSV seqntl files   */
/* Nov2002 - now change to presume RSV sequential files                 */
/*         - also presumes all recs minsize ??                          */
/*      IE - using random rec# probably wont work well for RSV/ISV files*/

/*Mar2003 - unions u1 & u2 moved here from switchx subfunction         */
/* - was getting erroneous 'usage before def' msg from win32 lcc/gcc   */
union INTC u1;                 /* work area unions to switch ends      */
union INTC u2;
union INTC u3;
union INTC u4;
/*Jan14/11 - add u3,u4 for z8 write BDW = RDW+4 (on every record) */

/*eject*/
/*uvhd-------------------------- RDW stuff -----------------------------*/

char rdwhdr[48];         /* work area to read 2, 4,or 8 byte header     */

int rdwhs = 0;           /* RDW header size from option z               */
/*Nov05/07 - variable length RDW option z verify z0,z2,z4,or z8         */
/* - now always assume recsize in 1st 2 bytes                           */
/*Sep09/08 - provide option z1 for little-end RDW                       */
/* z2=2bytehdr, z4=4bytehdr, z8=BDW/RDW 4+4, z0 deflt z4,               */
/* - remove prior z# considered lth of hdr                              */

/*Mar03/2022 - add option z16 RDW files hdrsize=total recsize vs datasize*/
/* z16 - subtract hdrsize in getrdwh, existing assumes hdrsize=datasize  */
/*     - write to add hdrsize, existing logic assuming hdr=datasize      */
/*     - write logic yet to do as of Mar03/2022                          */

/*Sep09/08 - use rdwhx = extra bytes for total recsize */
int rdwhx;             /* 2 for z2, 0 for z4 & z8    */
int rdwsize;           /* current record size                         */
                       /* extracted from the 2 or 4 byte hdr          */
int bdwsize;           /* current block size                          */
int rdwslot;           /* current record slot (hdr+data+fill to mult4)*/

int bdwrn;             /* rec# of last BDW (for rdwerr) */
UVi64 bdwfp;           /* fileptr of last */
char bdwsave[8];       /* last BDW/RDW    */
char bdwsavehex[20];   /* last BDW/RDW in hex */
char bdwcurhex[20];    /* current BDW/RDW in hex */

/*Apr27/11 - add BDW/RDW stats collected while building Index     */
/*         - bdwmin used to bypass BDW on later record get/search */
int  bdw1st;
int  bdwmin;
int  bdwmax;
int  bdwavg;
int  bdwcnt;
UVi64 bdwsum;
UVi64 bdwnext;     /* fileptr to next BDW (while building Index) */
char bdwmins[12]; /* min BDW string from loadindex() */

int  rdwmin;
int  rdwmax;
int  rdwavg;
UVi64 rdwsum;
char rdwmaxs[20]; /* capture from RCSmax= for atol() */

/* vhxbits for data2vhx() report ERRs ib BDW/RDW prefixes */
/* - 0x01 + 0x02 if EBCDIC input file                     */
short vhxbits = 0x01;
int  indexbuildtime;             /* Indexing elapsed time */

/*Jun12/11 - save prior BDW/RDW, ensure bdwmin not last BDW in file */
UVi64 filesizeb;        /* filesize - 32768 ensure bdwmin not last */
int rdwsizep;           /* prior rdwsize for error report */
int bdwsizep;           /* prior bdwsize for error report */

/*eject*/
/*------------------------ find Index tables ---------------------------*/
/*Jul09/10 - Index tables for find cmd for faster access to spcfd rec#s */
/*         - tables of fileptrs & record#s to 1,2,3% etc of filesize    */
/*         - built by find option 'i' (command 'fi')                    */
/*Nov21/10 - Index built automatically for varlth files (RDW,text,IDX3/8)*/

UVi64 fifp[102]; /* table of fileptrs for 1%,2%,...100%     */
int firn[102];   /* table of record#s for 1%,2%,...100%     */
int fii;         /* index for table build & lookup          */
int fii1;        /* switch set 2 when Index build completed */

/* fifp[102] initialized/calculated as follows */
/* fifp[0]=0, fifp[1]=1% fsize, fifp[2]=2% fsize,..., fifp[100]=100% fisze  */
/* firn[0]=0, firn[1]=rec# at 1% filesize,..., firn[100]=lastrec# in file   */

/*----------------------- find Index file I/O --------------------------*/
/*Apr20/11 - find Index file I/O                                        */
/* findex() - function to build Index & save it OR load it if existing  */
/* findexload - called if Index file exists same filename & filesize    */
/* findexsave - called to write Index to $UVTMPDIR/uvhdindex_filename   */

/* Index file records are formatted as follows                  */
/* #01:uvhdindex_X--------filename--------X                     */
/* #02:filesize=999999,fileptr1=999900,options=z8               */
/* #03:IndexCreated=yymmdd_hhmmss,BuildTimeSeconds=999          */
/* #04:BDWmin=99,BDWmax=9999,BDWavg=999,Blocks=999              */
/* #05:RCSZmin=99,RCSZmax=9999,RCSZavg=999,Records=9999         */
/* #06:1=0,2=99,3=999,4=999,...etc...                           */
/* #07:10=9999,11=9999,...etc...                                */

char finame[180];    /* filename built here */
FILE *fiptr;         /* fileptr from fopen  */
int  fiopn;          /* file open switch    */
struct stat fstatI;  /* file status structure */

/* constants/variables used to write index */
int  firn1;          /* find index record# */
char fir1id[20] = "#01:uvhdindex_";
char fir2fs[20] = "#02:filesize=";
char firec[180];     /* area to build or read index records */

char filesizeIs[20]; /* filesize from #02:filesize=...      */
UVi64 filesizeI;     /* filesize converted to long int      */

char indexfile[128];    /* index filename for msg $UVTMPDIR/uvhdindex_... */
char indexcreated[20]; /* create date saved for msg */


/*eject*/
/* trtprint.c - translate table - convert unprintable chars to periods   */
/*            - originally linked in from archive UVlib.a                */
/* 971107 - included here to make prgm self-contained for internet share */
/*Apr04/06 - changed unprintable char rep from period to carat 0x2E to 0x5E */
/*         - but did not like so changed back                               */

unsigned char trtprint[256] = {
0x2E,0x2E,0x2E,0x2E,0x2E,0x2E,0x2E,0x2E,0x2E,0x2E,0x2E,0x2E,0x2E,0x2E,0x2E,0x2E,
0x2E,0x2E,0x2E,0x2E,0x2E,0x2E,0x2E,0x2E,0x2E,0x2E,0x2E,0x2E,0x2E,0x2E,0x2E,0x2E,
0x20,0x21,0x22,0x23,0x24,0x25,0x26,0x27,0x28,0x29,0x2A,0x2B,0x2C,0x2D,0x2E,0x2F,
0x30,0x31,0x32,0x33,0x34,0x35,0x36,0x37,0x38,0x39,0x3A,0x3B,0x3C,0x3D,0x3E,0x3F,
0x40,0x41,0x42,0x43,0x44,0x45,0x46,0x47,0x48,0x49,0x4A,0x4B,0x4C,0x4D,0x4E,0x4F,
0x50,0x51,0x52,0x53,0x54,0x55,0x56,0x57,0x58,0x59,0x5A,0x5B,0x5C,0x5D,0x5E,0x5F,
0x60,0x61,0x62,0x63,0x64,0x65,0x66,0x67,0x68,0x69,0x6A,0x6B,0x6C,0x6D,0x6E,0x6F,
0x70,0x71,0x72,0x73,0x74,0x75,0x76,0x77,0x78,0x79,0x7A,0x7B,0x7C,0x7D,0x7E,0x2E,
0x2E,0x2E,0x2E,0x2E,0x2E,0x2E,0x2E,0x2E,0x2E,0x2E,0x2E,0x2E,0x2E,0x2E,0x2E,0x2E,
0x2E,0x2E,0x2E,0x2E,0x2E,0x2E,0x2E,0x2E,0x2E,0x2E,0x2E,0x2E,0x2E,0x2E,0x2E,0x2E,
0x2E,0x2E,0x2E,0x2E,0x2E,0x2E,0x2E,0x2E,0x2E,0x2E,0x2E,0x2E,0x2E,0x2E,0x2E,0x2E,
0x2E,0x2E,0x2E,0x2E,0x2E,0x2E,0x2E,0x2E,0x2E,0x2E,0x2E,0x2E,0x2E,0x2E,0x2E,0x2E,
0x2E,0x2E,0x2E,0x2E,0x2E,0x2E,0x2E,0x2E,0x2E,0x2E,0x2E,0x2E,0x2E,0x2E,0x2E,0x2E,
0x2E,0x2E,0x2E,0x2E,0x2E,0x2E,0x2E,0x2E,0x2E,0x2E,0x2E,0x2E,0x2E,0x2E,0x2E,0x2E,
0x2E,0x2E,0x2E,0x2E,0x2E,0x2E,0x2E,0x2E,0x2E,0x2E,0x2E,0x2E,0x2E,0x2E,0x2E,0x2E,
0x2E,0x2E,0x2E,0x2E,0x2E,0x2E,0x2E,0x2E,0x2E,0x2E,0x2E,0x2E,0x2E,0x2E,0x2E,0x2E
};

/* EBs2MFs - translation table - EBCDIC signs to Microfocus COBOL signs */
/*         - used by the 'zx' option of acum 'a 53(9zx)'                */
/* corrects for EBCDIC unpacked signs already translated to ASCII chars */
/* pos 0-9  {ABCDEFGHI 0x7B,0x41-0x49 convert to 0123456789 0x30-0x39   */
/* neg 0-9  }JKLMNOPQR 0x7D,0x4A-0x52 convert to pqrstuvwxy 0x70-0x79   */
unsigned char EBs2MFs[256] = {
0x00,0x01,0x02,0x03,0x04,0x05,0x06,0x07,0x08,0x09,0x0A,0x0B,0x0C,0x0D,0x0E,0x0F,
0x10,0x11,0x12,0x13,0x14,0x15,0x16,0x17,0x18,0x19,0x1A,0x1B,0x1C,0x1D,0x1E,0x1F,
0x20,0x21,0x22,0x23,0x24,0x25,0x26,0x27,0x28,0x29,0x2A,0x2B,0x2C,0x2D,0x2E,0x2F,
0x30,0x31,0x32,0x33,0x34,0x35,0x36,0x37,0x38,0x39,0x3A,0x3B,0x3C,0x3D,0x3E,0x3F,
0x40,0x31,0x32,0x33,0x34,0x35,0x36,0x37,0x38,0x39,0x71,0x72,0x73,0x74,0x75,0x76,
0x77,0x78,0x79,0x53,0x54,0x55,0x56,0x57,0x58,0x59,0x5A,0x5B,0x5C,0x5D,0x5E,0x5F,
0x60,0x61,0x62,0x63,0x64,0x65,0x66,0x67,0x68,0x69,0x6A,0x6B,0x6C,0x6D,0x6E,0x6F,
0x70,0x71,0x72,0x73,0x74,0x75,0x76,0x77,0x78,0x79,0x7A,0x30,0x7C,0x70,0x7E,0x7F,
0x80,0x81,0x82,0x83,0x84,0x85,0x86,0x87,0x88,0x89,0x8A,0x8B,0x8C,0x8D,0x8E,0x8F,
0x90,0x91,0x92,0x93,0x94,0x95,0x96,0x97,0x98,0x99,0x9A,0x9B,0x9C,0x9D,0x9E,0x9F,
0xA0,0xA1,0xA2,0xA3,0xA4,0xA5,0xA6,0xA7,0xA8,0xA9,0xAA,0xAB,0xAC,0xAD,0xAE,0xAF,
0xB0,0xB1,0xB2,0xB3,0xB4,0xB5,0xB6,0xB7,0xB8,0xB9,0xBA,0xBB,0xBC,0xBD,0xBE,0xBF,
0xC0,0xC1,0xC2,0xC3,0xC4,0xC5,0xC6,0xC7,0xC8,0xC9,0xCA,0xCB,0xCC,0xCD,0xCE,0xCF,
0xD0,0xD1,0xD2,0xD3,0xD4,0xD5,0xD6,0xD7,0xD8,0xD9,0xDA,0xDB,0xDC,0xDD,0xDE,0xDF,
0xE0,0xE1,0xE2,0xE3,0xE4,0xE5,0xE6,0xE7,0xE8,0xE9,0xEA,0xEB,0xEC,0xED,0xEE,0xEF,
0xF0,0xF1,0xF2,0xF3,0xF4,0xF5,0xF6,0xF7,0xF8,0xF9,0xFA,0xFB,0xFC,0xFD,0xFE,0xFF
};

/*eject*/
/* asc2ebc.c - ASCII to EBCDIC translation table     */
/*           - Owen Townsend, UV Software, June 1995 */
/* similar to ISO 8859-1, but defines some control chars undefined by ISO 8859*/
unsigned char asc2ebc[256] = {
0x00,0x01,0x02,0x03,0x37,0x2d,0x2e,0x2f,0x16,0x05,0x25,0x0b,0x0c,0x0d,0x0e,0x0f,
0x10,0x11,0x12,0x13,0x3c,0x3d,0x32,0x26,0x18,0x19,0x3f,0x27,0x1c,0x1d,0x1e,0x1f,
0x40,0x5a,0x7f,0x7b,0x5b,0x6c,0x50,0x7d,0x4d,0x5d,0x5c,0x4e,0x6b,0x60,0x4b,0x61,
0xf0,0xf1,0xf2,0xf3,0xf4,0xf5,0xf6,0xf7,0xf8,0xf9,0x7a,0x5e,0x4c,0x7e,0x6e,0x6f,
0x7c,0xc1,0xc2,0xc3,0xc4,0xc5,0xc6,0xc7,0xc8,0xc9,0xd1,0xd2,0xd3,0xd4,0xd5,0xd6,
0xd7,0xd8,0xd9,0xe2,0xe3,0xe4,0xe5,0xe6,0xe7,0xe8,0xe9,0xAD,0xe0,0xBD,0x5f,0x6d,
0x79,0x81,0x82,0x83,0x84,0x85,0x86,0x87,0x88,0x89,0x91,0x92,0x93,0x94,0x95,0x96,
0x97,0x98,0x99,0xa2,0xa3,0xa4,0xa5,0xa6,0xa7,0xa8,0xa9,0xc0,0x4F,0xd0,0xa1,0x07,
0x80,0x81,0x82,0x83,0x84,0x85,0x86,0x87,0x88,0x89,0x8A,0x8B,0x8C,0x8D,0x8E,0x8F,
0x90,0x91,0x92,0x93,0x94,0x95,0x96,0x97,0x98,0x99,0x9A,0x9B,0x9C,0x9D,0x9E,0x9F,
0xA0,0xA1,0x4A,0xA3,0xA4,0xA5,0xA6,0xA7,0xA8,0xA9,0xAA,0xAB,0xAC,0xAD,0xAE,0xAF,
0xB0,0xB1,0xB2,0xB3,0xB4,0xB5,0xB6,0xB7,0xB8,0xB9,0xBA,0xBB,0xBC,0xBD,0xBE,0xBF,
0xC0,0xC1,0x62,0xC3,0xC4,0xC5,0xC6,0xC7,0x74,0x71,0xCA,0xCB,0xCC,0xCD,0xCE,0xCF,
0xD0,0xD1,0xD2,0xD3,0xD4,0xD5,0xD6,0xD7,0xD8,0xD9,0xDA,0xDB,0xDC,0xDD,0xDE,0xDF,
0xE0,0xE1,0xE2,0xE3,0xE4,0xE5,0xE6,0xE7,0xE8,0xE9,0xEA,0xEB,0xEC,0xED,0xEE,0xEF,
0xF0,0xF1,0xF2,0xF3,0xF4,0xF5,0xF6,0xF7,0xF8,0xF9,0xFA,0xFB,0xFC,0xFD,0xFE,0xFF
};

/*Dec24/2014 - uvhd table replaced by srcf/asc2ebc.c        */
/*           - after updates of Dec24/14 re [ ] & cent sign */
/* Nov2002 - asc2ebc.c modified x'80'- x'FF' now neutral    */
/*         - asc2ebc2.c created all x'40' in x'80'-x'FF'    */
/*Sep02/12 - add accented chars: A circumflex x'C2'-x'62',  */
/*           E acute x'C9'-x'71', E grave x'C8'-x'74'       */
/*Oct14/14 - convert '|' x'7C' to x'4F (vs x'6A')           */
/*           to match Mar14/13 ebc2asc x'4F' to '|' x'7C'   */
/*Dec24/14 - change x'5B' to x'AD' '[' & x'5D' to x'BD' ']' */
/*         - see reverse changes in ebc2asc.c               */
/* see following notes in 'ebc2asc.c' applicable to this 'asc2ebc.c' */
/*Mar14/13 - change x'4F' from ']' to '|' for Cothern          */
/*Apr22/13 - change x'6A' from '|' to x'A6' (simotime alt '|') */
/* EBCDIC   '|' - ARC x'6A', Oracle x'BB', Simotime x'4F'      */
/* Oracle   '|' = x'BB', '[' = x'4A', ']' = x'5A', '!' = x'4F' */
/* Simotime '|' = x'4F', '[' = x'BA', ']' = x'BB', '!' = x'5A' */
/*Dec24/14 - add '[' & ']' 0xad & 0xbd code page 1047 IMMD     */
/*         - see reverse changes above in this asc2ebc.c       */
/*         - 0xA2 ASCII cent sign to 0x4A EBCDIC cent (vs '[') */

/*eject*/
/* ebc2asc.c - EBCDIC to ASCII translation table     */
/*           - Owen Townsend, UV Software, June 1995 */
/* similar to ISO 8859-1, but defines some control chars undefined by ISO 8859*/
/*Dec24/2014 - uvhd table replaced by srcf/ebc2asc.c        */
/*           - after updates of Dec24/14 re [ ] & cent sign */
unsigned char ebc2asc[256] = {
0x00,0x01,0x02,0x03,0x04,0x09,0x06,0x7f,0x08,0x09,0x0a,0x0b,0x0c,0x0d,0x0e,0x0f,
0x10,0x11,0x12,0x13,0x14,0x15,0x08,0x17,0x18,0x19,0x1a,0x1b,0x1c,0x1d,0x1e,0x1f,
0x20,0x21,0x22,0x23,0x24,0x0a,0x17,0x1b,0x28,0x29,0x2a,0x2b,0x2c,0x05,0x06,0x07,
0x30,0x31,0x16,0x33,0x34,0x35,0x36,0x04,0x38,0x39,0x3a,0x3b,0x14,0x15,0x3e,0x1a,
 ' ',0x41,0x42,0x43,0x44,0x45,0x46,0x47,0x48,0x49,0xA2, '.', '<', '(', '+', '|',
 '&',0x51,0x52,0x53,0x54,0x55,0x56,0x57,0x58,0x59, '!', '$', '*', ')', ';', '^',
 '-', '/',0xC2,0x63,0x64,0x65,0x66,0x67,0x68,0x69,0xA6, ',', '%', '_', '>', '?',
0x70,0xC9,0x72,0x73,0xC8,0x76,0x77,0x78,0x79, '`', ':', '#', '@',0x27, '=', '"',
0x80, 'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'i',0x8a,0x8b,0x8c,0x8d,0x8e,0x8f,
0x90, 'j', 'k', 'l', 'm', 'n', 'o', 'p', 'q', 'r',0x9a,0x9b,0x9c,0x9d,0x9e,0x9f,
0xa0, '~', 's', 't', 'u', 'v', 'w', 'x', 'y', 'z',0xaa,0xab,0xac,'[',0xae,0xaf,
0xb0,0xb1,0xb2,0xb3,0xb4,0xb5,0xb6,0xb7,0xb8,0xb9,0xba,0xbb,0xbc,']',0xbe,0xbf,
 '{', 'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H', 'I',0xca,0xcb,0xcc,0xcd,0xce,0xcf,
 '}', 'J', 'K', 'L', 'M', 'N', 'O', 'P', 'Q', 'R',0xda,0xdb,0xdc,0xdd,0xde,0xdf,
0x5c,0xe1, 'S', 'T', 'U', 'V', 'W', 'X', 'Y', 'Z',0xea,0xeb,0xec,0xed,0xee,0xef,
 '0', '1', '2', '3', '4', '5', '6', '7', '8', '9',0xfa,0xfb,0xfc,0xfd,0xfe,0xff
};
/* EBCDIC   '|' - ARC x'6A', Oracle x'BB', Simotime x'4F'       */
/* Oracle   '|' = x'BB', '[' = x'4A', ']' = x'5A', '!' = x'4F'  */
/* Simotime '|' = x'4F', '[' = x'BA', ']' = x'BB', '!' = x'5A'  */
/*Sep02/12 - add accented characters: A circumflex x'62'-x'C2', */
/*           E acute x'71'-x'C9', E grave x'74'-x'C8'           */
/*Mar14/13 - change x'4F' from ']' to '|' for Cothern           */
/*Apr22/13 - change x'6A' from '|' to x'A6' (simotime alt '|')  */
/*Dec24/14 - add '[' & ']' 0xad & 0xbd code page 1047 IMMD      */
/*         - 0x4A EBCDIC cent sign to 0xA2 ASCII cent (vs '[')  */

/*uvhd---------------------- translate tables -------------------------*/
/* neutral translate table - generated at program init                 */
Uchar trtn[256];
/* table to translate unprintables to chracters or periods(carats?)   */
Uchar trtc[256];
/* trtn copied to trtc & modified depending on translate options       */
/* - unprintables to blanks or periods(carats)                         */
/* - except for LFs, CRs, FFs, Tabs                                    */
/* - depending on options c1, c2, c4, c8                               */

/*eject*/
/*Dec19/10 - for getop, putop1, etc to suport acum & move numeric    */
/*------------------ ascii/ebcdic sign/zone constants ---------------*/
Uchar signasczp = 0x30;              /* ascii zoned  positive sign   */
Uchar signasczn = 0x70;              /* ascii zoned neg Microfocus   */
Uchar signebczp = 0xf0;              /* ebcdic zoned  positive sign  */
Uchar signebczn = 0xd0;              /* ebcdic zoned  negative sign  */
 char signseppa = 0x2B;              /* ascii separate negative sign */
 char signseppe = 0x4E;              /* ebcdic separate negative sign*/
 char signsepna = 0x2D;              /* ascii separate negative sign */
 char signsepne = 0x60;              /* ebcdic separate negative sign*/
Uchar zeroasc   = 0x30;              /* ascii zero                   */
Uchar nineasc   = 0x39;              /* ascii nine                   */
Uchar zeroebc   = 0xf0;              /*  ebcdic zero                 */
Uchar nineebc   = 0xf9;              /*  ebcdic nine                 */

/*9905 - setup signs for positive packed fields, depending on:        */
/*       run optn s1/s2, instrn optn s1/s2, or last + sign input      */
/*9906 - instrn option s0/s1/s2 now overrides run option s0/s1/s2     */
/*     - see 'setsigns' subfunction                                   */
Uchar signpp1 = 0x0c;                 /* ascii packed positive sign   */
Uchar signpp2 = 0x0f;                 /* ascii packed positive nosign */
Uchar signpn1 = 0x0d;                 /* ascii packed negative sign   */
                                      /* - never varies               */

/* ascii/ebcdic codes for current instrn operand op1 or op2           */
/* - setup by setcodes function depending on opdc                     */
/* - used for digit selection, sign detection & insertion, zone fill  */
Uchar zero = '0';                    /* ascii or ebcdic value stored  */
Uchar nine = '9';
Uchar signzp;                        /* ascii/ebcdic sign zoned pos   */
Uchar signzn;                        /* ascii/ebcdic sign zoned neg   */
 char signsepp;                      /* ascii/ebcdic separate sign neg*/
 char signsepn;                      /* ascii/ebcdic separate sign neg*/

/*note - signpp set from signpp1/signpp2/signpplast & options s0/s1/s2*/
Uchar signpplast;                    /* last pos packed sign input    */
Uchar signpp  = 0x0C;                /* packed pos sign for next putop1*/
Uchar signpn  = 0x0D;                /* ascii/ebcdic sign packed pos  */
                                     /* - neg pack sign never varies  */

/* sign indicator - all sign indications whether separate or zoned */
/* packed or unpacked are 1st detected & coded here as +/-         */
/* then tested & recoded on output to sepsign or zone in pkd/unpkd */
/* to allow flexibility converting input/output formats            */
char sign;                    /* sign byte '+' or '-'              */
                              /* indicator set by sep sign byte    */
                              /* or from zone/digit of units zn/pk */
Uchar signzone;               /* zoned  sign for current field     */
Uchar signpack;               /* packed sign for current field     */

/*eject*/
/*------------------- work areas for getop, putop, etc ----------------*/
char w1[256];
char w2[256];
char w3[256];
char w4[256];

/*eject*/
/*--------------- declare functions - unique to this program ------------*/

int getrec(short bits);
int getrecf(short bits);
int getrect(short bits);
int getrecv(short bits);
int getrecx(short bits);
int getlinr(int lsize);
void filterchar(unsigned char *string, int lsize);
void hexzone(char *out, char *in, int lsize);
void hexdigit(char *out, char *in, int lsize);
int search1(void);
int search2(char *recb, struct CA *cap);
int search3(char *recb, struct CA *cap);
int search4(char *recb, struct CA *cap);
int search234(char *recb, struct CA *cap);
int search34(char *recb, struct CA *cap);
int print1(int lsize);
int iprint1(int lsize);
int printrec(FILE *fptr, int lsize, int pspace);
int write1(void);
int drop1(void);
int writerec(void);
int update(void);
int move1(void);
int moven(void);
int seqgen(void);
int back1(void);
int backall(void);
int parse(void);
int chkrcsz(int recsiz0);
int calcrecs(int recsiz1, int rszdflt);
int check1(void);
int enum1(void);
int acum1(void);
int konvert(void);
int switchx(char *vhdr, int vrhs, int vrup48, short bits);
int trunc1(void);
int fileopen(void);
int fileclose(void);
int fileseek(char *emsg);
int getcbuf(void);
short boldpat(Uchar *linp, Uchar *linc);
short sorthtdd(char *tbl, int ecnt);
short bublsort(char *tbl, int el, int kd, int kl, int ecnt);
/* short writehist(void); */
short printRV(char *warnmsg);
int calcrsl(int rsiz, int fsiz);
int calcrsg(int rsiz, int fsiz);
short sortint2(int *tbl, int ents);
int scanrep(void);
int scanrep1(char *recb, struct CA *cap);
int trans(void);
int tran1(void);
int tolower2(char *str, int max, short bits);
int toupper2(char *str, int max, short bits);

/*eject*/
/*--------------- declare functions - originally in UVhdr.h -------------*/
/* 971107 - included here to make prgm self-contained for internet share */

char *atol1(int *result, char *in, char stopc, int max, short bits);
int atol2(char *str, char stopc, int max, short bits);
char *atol3(int *result, char *in, char *stopc, int max, short bits);
int sncopy(char *out, char *in, int max, short bits);
int stncopy(char *out,char *in,char stopc,int max,short bits);
int stmcopy(char *out,char *in,char *stopc,int max,short bits);
void errmsg(char *msg, char *inf1, char *inf2, int num, short bits);
int showhelps(char **hsp[], int hsmax);
int showhelp(char **helpmsgs, int max, short bits);
int waitreply1(char *reply, short quitctl);
int searchcm(char *data,char *matchchars,int max1,int max2,short bits);
void cnvdttm(char *dttm, time_t unixtime);
int hex2data(char *out, char *in, char stopc, int max, short bits);
int sortops(char *opsu, char *opsc, int *opsi, short bits);
int sortops4(char*opsu,char*opsc,int*opsi,char**opsp,char*opsb,short bits);
int toascii2(unsigned char *str, int max, short bits);
int toebcdic2(unsigned char *str, int max, short bits);
int toprint2(unsigned char *str, int max, short bits);
int sdscopy(char *out,char *in,int max,int *sds,char *stop,short bits);
int getlin0(FILE *fp, char *rec, int count, int stop, short bits);
int putlin0(FILE *fp, char *rec, int count, int stop, short bits);
short repchars(char *string, char *srcs, char *reps, int max, short bits);
/* int inithist(int argc, char **argv); */
int getdatetime(void);
int openfileX(char *xfname, char *xfid, FILE **xfptr, int *xfopn, short bits);
int closefileX
   (char *xfname, char *xfid, FILE **xfptr, int *xfopn, int *xfctr, short bits);
int upfileptr(int recsize1);
int setfileptr(UVi64 newfileptr);
int findex(void);
int findexsave(void);
int findexload(void);
int findrec(void);
int getrdwh(short bits);
int getrdwh2(short bits);
int getrdw(short bits);
int rdwerr(char *msg, short bits);

int setlock(int fd, int locktype, char *fname);
void waitlock(int *waitacum, char *fname, char *msg);

/*eject*/
/*Dec19/10 prototypes for getop, putop1, etc to suport acum & move numeric*/
void pack(char *out, char *in);
void unpack(char *out, char *in, short bits);
void tstsignp(char *wa, struct CA *carg);
void tstsignz(char *out, char *in, struct CA *carg);
void setsignp(char *wa);
void setsignz(char *out, char *in, int db1, char sgncd);
short bswap2(Uchar *ptr);
short bswap3b(Uchar *ptr);
short bswap3i(Uchar *ptr);
short bswap4(Uchar *ptr);
short bswap8(Uchar *ptr);
void setcodes(char code);
void setsigns(struct CA *carg);
UVi64 getop(char *recb, struct CA *carg, short bits);
void putop1(char *recb, struct CA *carg, UVi64 binarg);
int close1(void);
void data2vhx(char *chars,char *zons,char *digs,char *data,int lth
               ,unsigned char *ebc2asc, short bits);
void data2hex(char *out, char *in, int max, short bits);
int byte2rec(UVi64 fp1);

/*eject*/
/**************************** main ************************************/
int main(int argc, char **argv)
{
int ii,ll,ss,rr;               /* misc int (status, etc)         */
char *pp,*cc;                     /* misc char ptr                    */

getdatetime();                    /* get current date & time & format */

/* store options from user preferences env-var $UVHDROP first          */
/* - will later append any command line options (to override UVHDROP)  */
pp = getenv("UVHDROP");
if (pp)                           /* if env-var UVHDROP defined ?      */
  { strcpy(opsu,pp);
  }
ss = sortops4(opsu,opsc,opsi,opsp,opsb,0x10); /* sort/store alpha/num arrays*/
if (ss < 0)
  { errmsg("options invalid, must be alpha+numeric a1b2c300 etc"
            ,opsu,"",0,0x21);
  }

/* store users HOME dir for history file, or null = current dir if undef */
pp = getenv("HOME");
if (pp)
  { strcpy(homedir,pp);
  }

/* store users current working directory, or null = relative dir if undef */
pp = getenv("PWD");
if (pp)
  { strcpy(cwdir,pp);
  }

/* generate neutral translate table at x2100(256) or $trt           */
/* - for user to copy & modify before using trt instruction         */
for (ii=0; ii < 256; ii++)
     trtn[ii] = (Uchar) ii;

memset(blank256,' ',256);    /* init blanks for comparisons */

/*eject*/
/*May15/07 - test history file option j1 (may set in UVHDROP)        */
/*Feb04/09 - cmd history code removed (saved in src/uvhd_history.c)  */
/* if (opsi['j'-'a'] & 0x01) */
/*   { inithist(argc,argv);  */
/*   }                       */
/* writehist();              */
/* else ...                  */
    if (argc < 2)
      { showhelps(hsp,26);
        /* writehist();      write history file for any deletes*/
        /*Oct05/11 - show version, machine & H32/H64 after help screens*/
        printf("\nuvhd version=%s, machine=%s, H32/H64=%s\n\n"
               ,uvversion,machine,H32H64);
        exit(1);
      }
    else /* command line args (filename [& options] are present     */
      { strcpy(fn1,argv[1]);            /* store filename from arg1 */
        if (argc > 2)
          { strcat(runops,argv[2]);     /* concat options from arg2 */
          }
      }

/* store full pathname (whether history file or command line)       */
strcpy(fn1a,fn1);             /* presume full path already existing */
if ((fn1[0] != '/') && (fn1[0] != '\\'))
  { if (cwdir[0])             /* if $PWD was stored OK              */
      { strcpy(fn1a,cwdir);
        strcat(fn1a,slashs);
        strcat(fn1a,fn1);
      }
    else
      { strcpy(fn1a,fn1);
      }
  }

/* calc length of smso & rmso (in case user modifies stringa)          */
smsol = strlen(smso);
rmsol = strlen(rmso);

/*Jun12/07 - optn 'n' multi-recs/scrn fix restore fileptr to 1st on screen */
/*         - commands other than browse, rec#, byte# (write, print, etc)   */
/*         - not working try init cmd to blank (see test at end screen)    */
opcmd[0] = ' ';

/*eject*/
/* create filenames for print & write commands (in case needed)        */
/* print filename = $UVTMPDIR/inputfilename_yymmdd_hhmmssP             */
/* write filename = $UVTMPDIR/inputfilename_yymmdd_hhmmssW             */
/* drop filename  = $UVTMPDIR/inputfilename_yymmdd_hhmmssD             */

/* 1st extract root filename from input filename (removing prefix dirs)*/
strcpy(fn1b,fn1);                 /* presume no prefix directories     */
pp = strrchr(fn1,'/');            /* find last '/' (if any)            */
cc = strrchr(fn1,'\\');           /* find last '\' (if any)            */
if (pp)
  { strcpy(fn1b,pp+1);            /* copy root filename after last '/' */
  }
else if (cc)
  { strcpy(fn1b,cc+1);            /* copy root filename after last '/' */
  }

/* if WinDOS, convert any '/' to '\' in filename for write, print, etc */
#ifdef DWIN
repchars(fn1b,"/","\\",80,0);
#endif

/*Feb05/09 - common functions openfileX & closefileX for P,I,W,V,D files*/
/*         - chg P/I/W/V/D cmds to open & close output file on each cmd */
/*         - cmd line option w1 to write 1 combined file REMOVED 2010   */
/*         - filename format as follows:                                */
/* $UVTMPDIR/inputfilename_yymmdd_hhmmssX               <-- X=P/I/W/V/D */
/* $UVTMPDIR/inputfilename_yymmdd_   - basefilename + time + X each open*/

/* create basefilename now at prgrm init & when files opened on each cmd*/
/* - will copy to pfname,ifname,wfname,etc & append time + ID P/I/W/V/D */
/*May03/11 - provide $UVTMPDIR define subdir for print,write,drop,index,etc*/
strcpy(tmpdir,"tmp");          /* presume UVTMPDIR not specified */
pp = getenv("UVTMPDIR");
if (pp)
  { if (pp[0])
      { strcpy(tmpdir,pp);
      }
  }
/*May03/11 - create tmpdir at program init if not already present */
/* create tmp dir if not already present                             */
ss = stat(tmpdir,&tmpstat);
if (ss < 0)
  { sprintf(mkdirtmp,"mkdir %s",tmpdir);
    system(mkdirtmp);
  }

/* create basefilename for P,I,W,V,A,C,D tmp files */
strcpy(basename,tmpdir);
strcat(basename,slashs);
strcat(basename,fn1b);
strcat(basename,"_");
strcat(basename,date6);
strcat(basename,"_");

/*eject*/
/* create filename for Find Index file for findexsave() & findexload() */
strcpy(finame,tmpdir);         /* $UVTMPDIR or tmp              */
strcat(finame,"/uvhdindex_");  /* append Find Index prefix name */
strcat(finame,fn1b);           /* append uvhd input datafile    */

/* setup iprint command + filename, default command to uvlp12        */
/* - override if export UVHDPRINT=lp or uvlp12L, etc                 */
pp = getenv("UVHDPRINT");
if (pp)
  { strcpy(prtcmd,pp);
  }

/* setup iprint cmd Wide/Landscape + filename, default to uvlp12L     */
/* - override if export UVHDPWIDE=lp or uvlp12L, etc                  */
pp = getenv("UVHDPWIDE");
if (pp)
  { strcpy(prtcmdW,pp);
  }

/*eject*/
/*uvhd---------------------- process options ----------------------------*/
/* process options - concat options from user or history onto any UVHDROP*/
/* - sort by alpha, store alpha array,& convert any num prmtrs to integer*/
strcat(opsu,runops);            /* concat any this run ops onto any UVHDROP */
ss = sortops4(opsu,opsc,opsi,opsp,opsb,0x10); /* sort/store alpha/num arrays*/
if (ss < 0)
  { errmsg("options invalid, must be alpha+numeric a1b2c300 etc"
            ,opsu,"",0,0x21);
  }

/* default option i3 if options i1 or i2 unspecified on DWIN               */
/* i1 inhibits Bold search patterns, i2 inhibits BOLD warns re fsize/rsize */
/*Jan10/11 - option chg to i from b now used for Begin display fileptr     */
#ifdef DWIN
if (opsc['i'-'a'] == 0)          /* option i unspcfd ?         */
  { opsi['i'-'a'] = 3;           /* force inhibit Bold on DWIN */
  }
#endif

/* store recsize option */
/*Nov11/07 - default to 256 if not spcfd or too big */
/*Jul08/12 - if textfile default to 512 */
/*Dec16/13 - for text files, default recsize 8000  */
/*Sep02/14 - change default to 9984 (vs 8000) */
rszo = opsi['r'-'a'];
if (rszo < 1 || rszo > RMAX)
  { if (opsc['t'-'a'])
      { rszo = 9984;
      }
    else
      { rszo = RDFLT;
      }
  }
/*Nov11/07 - init rsz1p last valid recsize to option r (or 256) at init*/
rsz1p = rszo;

/* store line width option - default if not spcfd or too big */
lszo = opsi['l'-'a'];
if (lszo < 1 || lszo > LMAX)
    lszo = LDFLT;

/* setup units scale depending on option g1/g0 1/0 relative         */
/*Aug16/06 - option changed to 'g' from 'z' (now RDW varlth filetype*/
if (opsi['g'-'a'] & 0x01)
    memcpy(scaleu0,"1234567890",10);
else
    memcpy(scaleu0,"0123456789",10);

/* now duplicate units scale for max line length 1030                */
for (ii=0; ii < 1030; ii+=10)
  { memcpy(scaleu1+ii,scaleu0,10);
  }

/*eject*/
/*Dec07/10 - store parse EBCDIC constant type default if optn a   */
/*         - store Ebcdic blank for move/updt clear excess op1 lth*/
if (opsc['a'-'a'])
  { strcpy(AEC,"=e");
    AEB = 0x40;
    vhxbits |= 0x02;    /* data2vhx bits to trnslt EBCDIC to ASCII */
  }

/* setup tens scale depending on option g1/g0 1/0 relative         */
/*Aug16/06 - option changed to 'g' from 'z' (now RDW varlth filetype*/
memset(scalet1,' ',1030);                /* init to all blanks     */
if (opsi['g'-'a'] & 0x01)
 { for (ii=0; ii <= 1030; ii+=10)
     { sprintf(scalet0,"%4d",ii+10);
       memcpy(scalet1+6+ii,scalet0,4);
     }
 }
else
 { for (ii=0; ii <= 1030; ii+=10)
     { sprintf(scalet0,"%4d",ii+10);
       memcpy(scalet1+7+ii,scalet0,4);
     }
 }

/* setup max lines/screen from option 'm', or set default if m unspcfd */
linmax = opsi['m'-'a'];
if (linmax <= 0)
    linmax = 24;

/*Aug16/06 - inhibit warnmsg 'filesize not multiple of recsize' */
/*         - if varlth options 'v', 'z', 't', 'x'               */
if ((opsc['v'-'a']) || (opsc['z'-'a']) || (opsc['t'-'a']) || (opsc['x'-'a']))
  { opsi['e'-'a'] = 3;
  }

/*Dec16/20 - add option t1,2,4,8 tchar replace unprintables on hex charline*/
/*         - t1 '.' (default), t2 ':', t4 '^', t8 '_'                      */
tchar = '.';     /* set default '.' */
if (opsi['t'-'a'] & 0x02)
  { tchar = ':';
  }
else if (opsi['t'-'a'] & 0x04)
  { tchar = '^';
  }
else if (opsi['t'-'a'] & 0x08)
  { tchar = '_';
  }

/*eject*/
/* call subfunction to open file (also used by truncate command)       */
fileopen();                   /* open file, store filesize, etc        */

/* now add new filename & options onto the end of the history file table */
/* & write the file back to disc, drop off 100 entries if > 400 entries  */
/* create new entry for history file --> yymmdd filename options <-- */
/*Feb04/09 - cmd history code removed (saved in src/uvhd_history.c)  */
/*         - about 20 lines of code (histadd) removed                */

/*May28/11 - store search progress message interval record count */
/*         - from option p default 1 million                     */
if (opsi['p'-'a'])
  { pmsgcnt = opsi['p'-'a'];
  }
else
  { pmsgcnt = 1000000;
  }

/*eject*/
/*Dec2004 - test option 'v' (was x) Micro Focus COBOL IDXFORMAT8  */
/*Feb05/09 - combine getrev & old getrecx(IDXFORMAT) as getrecv  */
if (opsc['v'-'a'])
  { ftype = 'v';
    /* v options: v1=filehdr,v2=deleted,v4=actixe data,v8=all (dflt=v4)*/
    /* - default to v4 if not explicit                                 */
    if (opsi['v'-'a'] == 0)
        opsi['v'-'a'] = 4;

/* - read file prefix (1st 128 bytes) to get max rcsz, min rcsz, etc   */
/* - will read remainder of file header later, depending on header size*/
/* - verify file is IDXFORMAT_ (1st 4 bits 0x30)                       */
/* - store max rcsz, min rcsz, file header size                        */

    rsz1 = read(fd1,vfhdr,128);          /* read 1st 128 of file       */

    /* verify file hdr rec valid                                       */
    if ((vfhdr[0] & 0xF0) == 0x30)
      { ;
      }
    else
      { errmsg("file header invalid (1st 4 bits not 0x30)",fn1,"",0,0x21);
      }
    /* store max rcsz, min rcsz                            */
    vrmax = switchx(vfhdr+54,4,4,0x00); /* store max rcsz  */
    vrmin = switchx(vfhdr+58,4,4,0x00); /* store min rcsz  */
    /* int switchx(char *vhdr, int vrhs, int vrup48, short bits) */
    /*Feb26/16 - switchx 4 args, arg2 hdrsiz, arg3 roundup4/8, arg4 inhib +hdrsiz*/

    /* store rec hdr size vrhs=4 if macrcsz > 4095, else vrhs=2         */
    /*Mar19/08 - uxcopy rcs=4095 creates 4 byte rechdr, rcs=4094 2 bytes*/
    /* - so change test below from (vrmax > 4095) to (vrmax >= 4095)    */
    /* presume vrmax < 4096 */
    vrhs = 2;
    wrhs = 2;
    vrup48 = 4;
    /*Sep21/16 - for option v & maxrec >4095, add w8 roundup multiple 8 */
    if (vrmax > 4095)
      { vrhs = 4;
        wrhs = 4;
        if (opsi['w'-'a'] & 0x08)
          { vrup48 = 8;
          }
      }

/*eject*/
    /* call switchx to calc slot size of file header record            */
    switchx(vfhdr,vrhs,vrup48,0x00);       /* calc slsot size of header*/
    vfhs = vrslot;                         /* save file header size    */

    /* call subfunctn to check rcsz & calc number of records in file   */
    /* set rcsz = min rcsz for ISAM IDXFORMAT8                         */
    /*Jun20/07 - changed to max recsize, varlth files often all max    */
    rszo = vrmax;
    chkrcsz(rszo);

    /* reset ptr = 0 in case filehdr rec display desired (option v1)   */
    fileptr = 0;

    /* read any remaining size of file header (128 read above)         */
    vfxtra = (vrslot - 128);               /* calc size yet to be read */
    if (vfxtra > 0)
      { rsz1 = read(fd1,vfhdr+128,vfxtra); /* read remainder of header */
      }
  }

/*eject*/
/*Feb20/16 - add option 'x' for STL files */
else if (opsc['x'-'a'])
  { ftype = 'x';
    /* x options: x2=sequential, x4=Indexed, x8=Relative */
    if ((opsi['x'-'a'] & 0x0E) == 0)
      { errmsg("STL option must be x2=Seqntl, x4=Indexed,or x8=Relative",fn1,"",0,0x21);
      } 

    if (opsi['x'-'a'] & 0x02)
      { rsz1 = read(fd1,vfhdr,112);         /* read STLS hdr */
        /* verify file hdr STLS */
        if (memcmp(vfhdr+8,"STLS",4) != 0)
          { errmsg("option x2 file header NOT STLS in 8-11",fn1,"",0,0x21);
          }
        vrmax = switchx(vfhdr+68,4,0,0x00); /* store STLS max rcsz  */
        vrmin = switchx(vfhdr+76,4,0,0x00); /* store STLS min rcsz  */
        /* int switchx(char *vhdr, int vrhs, int vrup48, short bits) */
        /*Feb26/16 - switchx 4 args, arg2 hdrsiz, arg3 roundup4/8, arg4 inhib +hdrsiz*/
        vrhs = 4; vrup48 = 0;
      }
    else if (opsi['x'-'a'] & 0x04)
      { rsz1 = read(fd1,vfhdr,2048);        /* read STLI hdr */
        /* verify file hdr STLI */
        if (memcmp(vfhdr+8,"STLI",4) != 0)
          { errmsg("option x4 file header NOT STLI in 8-11",fn1,"",0,0x21);
          }
        vrmax = switchx(vfhdr+156,4,0,0x00);  /* store STLI max rcsz */
        vrmin = switchx(vfhdr+164,4,0,0x00);  /* store STLI min rcsz */
        vrhs = 4; vrup48 = 0;
        vrsys = switchx(vfhdr+140,4,0,0x00);  /* system record size  */
        /*Mar30/18 - STL 5.1 system recsize now 32K (vs 4K assumed)  */
      }
    else if (opsi['x'-'a'] & 0x08)
      { rsz1 = read(fd1,vfhdr,2048);        /* read STLI hdr */
        /* verify file elative hdr (same as Indexed) */
        if (memcmp(vfhdr+8,"STLI",4) != 0)
          { errmsg("option x8 file header NOT STLI in 8-11",fn1,"",0,0x21);
          }
        vrmax = switchx(vfhdr+156,4,0,0x00); /* store STLR max rcsz  */
        vrmin = switchx(vfhdr+164,4,0,0x00); /* store STLR min rcsz  */
        vrhs = 4; vrup48 = 0;
        vrsys = switchx(vfhdr+140,4,0,0x00);  /* system record size  */
        /*Mar30/18 - STL 5.1 system recsize now 32K (vs 4K assumed)  */
      }
    else
      { errmsg("STL option must be x2=Seqntl,x4=Indexed,or x8=Relative XXX",fn1,"",0,0x21);
      }
       
    /* call subfunctn to check rcsz & calc number of records in file   */
    rszo = vrmax;
    chkrcsz(rszo);
    fileptr = 0;
  }

/*eject*/
/*Jul02/07 - store write record hdr size for file type z         */
/*Nov05/07 - save RDW rechdrsize from option z & verify 0,2,4,8  */
/* - optn z value is extra bytes reqd to reach begin next rec    */
/*Sep09/08 - provide option z1 for little-end RDW                */
/* z2=2bytehdr, z4=4bytehdr, z8=BDW/RDW 4+4, z0 deflt z4,        */
/* - remove prior z# considered lth of hdr                       */

/*Mar03/2022 - add option z16 RDW files hdrsize=total recsize vs datasize */
/* z16 - subtract hdrsize in getrdwh, existing assumes hdrsize=datasize   */

else if (opsc['z'-'a'])
  { ftype = 'z';
    /* validate option z & set rdwhs = RDW hdr size             */
    /*Sep09/08 - also set rdwhx = extra bytes for total recsize */
    if (opsi['z'-'a'] & 0x02)
      { rdwhs = 2; 
        rdwhx = 2; 
      }
    else if (opsi['z'-'a'] & 0x04)
      { rdwhs = 4; 
        rdwhx = 0; 
      }
    else if (opsi['z'-'a'] & 0x08)
      { rdwhs = 4;                 /*May17/10 - chg from 8 to 4 */
        rdwhx = 0; 
      }
    /*May17/10 - change optn z8 to BDW/RDW, bypass BDW in getrdw() */
    /*         - BDW/RDW 4+4, BDW bypasswd, else like z4           */
    else
      { errmsg("valid option z=z2,z4,z8(big-end) or z3,z5,z9(little-end)"
               ,opsu,"",0,0x21);
      }
    wrhs = 2;   /* wrhs used to offset translates etc */
  }
else if (opsc['t'-'a'])
  { ftype = 't';
    /* calcs for variable length text records (option 't') */
    chkrcsz(rszo);
    /*Jan16/15 - allow multi-byte record terminator in hex t=xxxx */
    /* uvhd filename t=FF01  <-- example for EBCDIC End-Of-Record */
    /*Mar21/19 - ignore t=string if not at least 2 hex chars      */
    if ((opsp['t'-'a']))
      { strcpy(eoridc,opsp['t'-'a']);  /* capture any t=string */
        eorlc = strlen(eoridc);        /* get length if any ?  */
        if (eorlc > 1)
          { eorlx = hex2data(eoridx,eoridc,'a',eorlc,0x03);
            if (eorlx > 6)
              { errmsg("option t=EORstring > max 6 hex bytes",opsu,"",0,0x21);
              }
          }
        else
          { ; } /*Mar21/19 - ignore t=string if not at least 2 hex chars */
      }
  }

/*eject*/
else
  { ftype = 'f';
    /* calcs for fixed lth records only                                   */
    /* call subfunctn to check rcsz & calc number of records in file      */
    /* - also called by command 'R' (change rcsz w/o quit & cmd line optn */
    chkrcsz(rszo);

    /*May25/10 - for fixed length we can set ptr to last rec at prgm init  */
    /*         - to cause EOF msg on 1st time displayed                    */
    fileptr1 = (filesize - rszo);
  }

/*Oct08/11 - edit fileptr1 for printf's %ld or %lld for 64/32bit HP gcc */
sprintf(fpedit1e,E64,fileptr1);

/*May21/10 - calc recsize to clear at begin each rec */
rszox = (rszo + 256); 
rszoy = (rszo + 128); 

/*Apr29/11 - simplify f1 dflt build varlth, f0 no build, f2 force build  */
/*Sep08/12 - default NO Index build for Text as well as Fixed records */
/*Feb22/13 - chnage default back to build index for Text files */
/*Feb26/16 - allow findex for all varlth files including STL files */
if ((ftype != 'f') && (!(opsc['f'-'a'])))
  { opsc['f'-'a'] = 'f';   /* default f1 build index for varlth */
    opsi['f'-'a'] = 1;
  }
if (opsi['f'-'a'] & 0x01)  /* build index ? (varlth default)   */
  { findex();              /* - will force rebuild if f2+f1=f3 */
    vrtyp2 = '\0';         /*Sep15/17 clear read unused space */
    IndexBuild = 0;        /* reset for STL x4y4j4 deletes only */
  }

/*May03/11 - fix 'f0' (inhibit build index), set fii1=3 vs fii1=2 if Index */
/*         - so getrdw won't assume Index, so findrec won't assume Index*/
if ((opsc['f'-'a']) && (opsi['f'-'a'] == 0))
  { fii1 = 3;
  }

/*Oct08/16 - edit deleted count for displays from Index file save/load */
/* - set edited string null if dcount2 zero for files w/o delete possible */
if (dcount2 == 0)
  { dcounts2[0] = '\0';
  }
else
  { sprintf(dcounts2,"deleted=%d",dcount2);
  }

/*eject*/
fileptr = 0;       /* restore fileptr to begin file         */
upnext = 0;        /* inhibit pre-increment to get 1st rec  */
recnum1 = 1;       /* init rec# for 1st rec in file         */
/*Apr19/16 - recnum1 init 1 since upnext inhibits ++ 1st get*/
setfileptr(0);     /* set fileptr 0 & recnum++ if FixedLth  */

/*Jun05/10 - test optn 'b' fileptr of 1st record to display */
/*         - changed from first record# to display          */
/*         - for uvhdcob to work with varlth files          */
/*Jan10/11 - option changed to b, f0 now inhibit find index build */
/*Aug23/11 - fix rec# calc from byte# passed from uvhdcob exit to uvhd*/
/*         - depending on filetype RDW or fixed length                */
if (opsi['b'-'a'] > 1)
  { fileptr = opsi['b'-'a'];        /* option b fileptr Begin display */
    if (ftype == 'z')
      { byte2rec(fileptr);
      }
    else
      { recnum0 = (fileptr / rszo); /* calc rec# assuming fixed lth */
        recnum1 = (recnum0 + 1);    /* calc recnum1 1 relative      */
      }
  }

/*May28/11 - search op max check based on max recsz not current recsize*/
rcsmax = rszo;
if ((ftype != 'f') && (rdwmax))
  { rcsmax = rdwmax;
  }

/*eject*/
/*uvhd---------------------- begin record -----------------------------*/
/* get next sequential record - or depending on fileptr                */

bgnrec:

rsz1 = getrec(0);                     /* get next record           */

if (rsz1 <= 0)                        /* error or EOF ?            */
  { goto endscreen;
  }

/* if at begin file (rec#1) - output hdr with file statistics */
/*Jan10/11 - insert uvhd & version date               */
/*Oct08/16 - add deleted count to displays & Index file save/load */
if ((fileptr == 0) || (recnum1 <= 1))
  { printf("\nnow=%s uvhd %s %s\n",todttm,fn1a,opsu);
    printf("version=%s records=%d %s rsize=%d fsize=%s%s\n",
                 uvversion,rcount,dcounts2,rsz1,fsedit,rmndrs);
  }

/*eject*/
/*uvhd-----------------------begin screen-------------------------------*/
bgnscreen:

linctr = 0;                        /* clear line ctr for current screen */

/*Apr05/07 - test option o# to display scale every # records */
/*May15/07 - option changed from 'i' to 'o'                  */
/* calc remainder (recnum1 % optn o) only if optn o spcfd    */
recnum1o = 1;     /* presume no option o for test below      */
if ((opsi['o'-'a']) && (recnum1 > 1))
  { recnum1o = (recnum1 % opsi['o'-'a']);
  }
if (recnum1o == 1)
  { /* test option s1 - space prior to scale  */
    if (opsi['s'-'a'] & 0x01)
      {  printf("\n");
         linctr++;
      }

    /* set scalesize = recsize if scalesize > recsize */
    lszh = lszo;
    if (lszh > rsz1)
      { lszh = rsz1;
      }

    /*Mar08/2022 - set scale size 2 bytes extra if file type z16 (z2+z16)*/
    /*Mar11/2022 - now NOT Required after further testing */
    /* if (opsi['z'-'a'] & 0x10) */
    /*   { lszh +=2;             */
    /*   }                       */

    /* output screen hdr record column scale, 0/1 relative by option g0/g1  */
    sncopy(scalet2,scalet1,lszh,0x03);      /* setup scale truncated at lszh*/
    sncopy(scaleu2,scaleu1,lszh,0x03);
    printf("            %s\n",scalet2);    /* tens scale fileptr removed   */
    printf("r#%9d %s\n",recnum1,scaleu2);  /* units scale                  */
    linctr += 2;                           /* incrmnt line ctr for scale   */
    /*Apr2004 - option s8 added to space after scale  */
    if (opsi['s'-'a'] & 0x08)
      {  printf("\n");
         linctr++;
      }
  }
else
  {  printf("\n");        /* space 1 vs print scale */
     linctr++;
  }

/*eject*/
/*uvhd------------------- begin line -----------------------------------*/
bgnline:
/* get next line & convert to vertical hex display format               */
/* getlinr - extracts next display segment from current record          */
/*         - returns 0 if eof, else number of bytes in current segment  */

lsz1 = getlinr(lszo);             /* get next segment from record        */
if (lsz1 <= 0)
  { /* test option s4 - space after record (prior to prompt at endscreen) */
    if (opsi['s'-'a'] & 0x04)
      {  printf("\n");
         linctr++;
      }
    goto endscreen;
  }

memcpy(linc,linr,lszo);            /* copy for translate/filter (LMAX)  */

/* test option 'a' to translate character line from EBCDIC to ASCII   */
/* note - bit 0x04 converts non-print EBCDIC chars to periods(carats) */
if (opsc['a'-'a'])                   /* option 'a' active ?           */
  { toascii2((Uchar*)linc,lszo,0x04);  /* translate EBCDIC to ASCII (LMAX)*/
  }

/*Apr11/10 - test inhibit display of blank segments (except 1st) */
/*Sep11/15 - option d1 also inhibit null segments */
if ((opsi['d'-'a'] & 0x01) && (rii1))
  { if (memcmp(linc,blank256,lsz1) == 0)
      { goto bgnline;
      }
    if (memcmp(linr,null256,lsz1) == 0)
      { goto bgnline;
      }
  }

/* now convert data line to 3 lines for vertical hex display            */
/* - presuming full 64 bytes per line (will compensate below)           */
filterchar(linc,lszo);            /* format line #1 - character display */
hexzone(linz,linr,lszo);          /* format line #2 - hex zones display */
hexdigit(lind,linr,lszo);         /* format line #3 - hex digits display*/

/* linc,lind,linz have assumed 64 byte lines   (null in 65th)         */
/* now insert the array nulls again - based on length returned by     */
/* getlinr which may be < 64 if optn 'l' or optn 'f' or eof           */
   linc[lsz1] = '\0';
   linz[lsz1] = '\0';
   lind[lsz1] = '\0';

/* highlight last found search pattern, if any & not inhibited by optn b1 */
boldpat(linp,linc);              /* copy char data inserting highlight  */

/* test option s2 - space before data lines (3 line hex or 1 line char) */
if (opsi['s'-'a'] & 0x02)
  { printf("\n");
    linctr++;                          /* increment screen line ctr */
  }

/*eject*/
/*--------- now ready to print the 3 vertical hex lines ----------------*/
/*Nov2001 - fileptr removed on tens scale line, now insert on data segments*/
/*Dec2001 - always display fileptr on 1st segment of record             */
/*     d0 - display record dsplcmnts on following segments of record    */
/*     d2 - display file dsplcmnts on all segments of record            */

/* display character line of current segment of current record          */
/*May27/10 - default record dspls all segments vs fileptr at recdsp 0   */
/*           option d4 - show file dsplcmnts on all segments            */

/*Oct06/10 - show file dsplcmnt on character line of 1st segment       */
/*         - record dsplcmnt on other segments unless option d4        */
/*Oct08/11 - edit fileptr for printf's %ld or %lld for 64/32bit HP gcc */
if (rii1 == 0)
  { sprintf(fpedit2,E64,fileptr);          /* edit allowing 32/64 bits */
  }
else
  { if (opsi['d'-'a'] & 0x04)
      { sprintf(fpedit2,E64,fileptr+rii1); /* file dsplcmnts on all segs*/
      }
    else
      { sprintf(fpedit2,E64,rii1);         /* record dsplmnts on segs 2+*/
      }
  }
printf("%11s %s\n",fpedit2,linp);
linctr++;                              /* increment screen line ctr     */

/* show zones & digits line depending on option 'h'                       */
/* display depending on option 'h' & < 0x20 or > 0x7E (allow CRLF last2)  */
/*Nov11/11 - change options */
/* h0 = hex (default), h1 = force chars only, h2 = auto determine by data */
if (opsi['h'-'a'] & 0x01)
  { ; }
/* else if ((opsi['h'-'a'] & 0x02) && (xchar < 1)) */
/*Mar04/13 - fix h2 cmd not forcing hex - disable h2 & xchar test above */
else
  { printf("            %s\n",linz);
    printf("            %s\n",lind);
    linctr += 2;        /* increment for zones/digits */
  }

/* test line ctr for screen full - depends on hex/char/sp1/sp2      */
if (linctr >= linmax)
    goto endscreen;
goto bgnline;

/*eject*/
/*uvhd------------------ end of current screen -------------------------*/
endscreen:
/* return point for some error conditions */
reprompt:

/* print any stored opmsg & opmsg2 & clear to prevent repeat      */
/* msgs such as search found locations stored in subrtns & stored */
/* - for display after the relevant screen is displayed above     */
if ((opmsg1[0]) || (opmsg2[0]) || (opmsg3[0]))
  { if (opmsg1[0])
      { printf("%s",opmsg1);
        strcpy(oppmsg1,opmsg1);  /*Mar19/19 - save for any folwng printrec*/
        opmsg1[0] = '\0';
      }
    if (opmsg2[0])
      { printf("%s",opmsg2);
        strcpy(oppmsg2,opmsg2);
        opmsg2[0] = '\0';
      }
    if (opmsg3[0])
      { printf("%s",opmsg3);
        strcpy(oppmsg3,opmsg3);
        opmsg3[0] = '\0';
      }
    /* - generate divider line to empahsize preceding errmsgs */
    printf("--------------------------------------------------------------\n");
  }

/* issue warning msg if recsize not divisible evenly into filesize */
if ((filesize > filesiz0) && (ftype == 'f') && (opsi['e'-'a'] & 0x01) == 0)
  { filextra2 = filextra;        /* prevent warning if UVi64 long long*/
    sprintf(warnmsg1,"filesize NOT multiple of recsize, %d bytes remain\n"
                     ,filextra2);
    printRV(warnmsg1);           /* display msg in reverse video */
  }

/* test for option 'n' (multi-recs per screen) when last cmd null=browse*/
if ((opsi['n'-'a']) && (opcmd[0] <= ' '))
  { rpsctr++;                    /* increment recs per screen ctr  */
    if (rpsctr < opsi['n'-'a'])
      { /* fileptr += rsz1;  advance ptr to next record */
        /*May25/10 - replace above with upnext=1        */
        upnext = 1;              /* ensure advance to next record  */
        goto bgnrec;             /* return to next rec w/o prompt  */
      }
    else
      { rpsctr = 0;
        fileptro1 = fileptr;     /* save fileptr of 1st on screen  */
                                 /* for restore on non-browse cmds */
      }
  }

/*eject*/
/* prompt operator for next command */
/*May27/10 - replace filename on stats (at bottom screen) with filepointer */
/*Oct06/10 - change field order (was rec#,rsize,rcount,fsize,fptr) */
/*Oct08/16 - add deleted count to displays & Index file save/load  */
printf("rec#=%d rsize=%d fptr=%s fsize=%s records=%d %s"
              ,recnum1,rsz1,fpedit1,fsedit,rcount,dcounts2);

/*Mar18/03 - display more info if filesize not multiple of recsize      */
/*           when on 1st record in file unless inhibited by option e2   */
if ((filesize > filesiz0) && (fileptr == 0) && ((opsi['e'-'a'] & 0x02) == 0))
  { ll = calcrsl(rsz1,filesize);        /* calc next multiple < recsize */
    rr = calcrsg(rsz1,filesize);        /* calc next multiple > recsize */
    printRV("\nIf Text file (with LineFeeds),  quit/restart with option 't'");
    printRV("\nIf Variable (IDXFORMAT3/8,RDW), quit/restart with optn v,or z");
    sprintf(warnmsg1
    ,"\nIf Fixed length records, change recsize now via command 'R' (R%d current)",rsz1);
    printRV(warnmsg1);
    sprintf(warnmsg1
    ,"\nnext evenly divisible record sizes lower/higher are --> R%d/R%d"
    ,ll,rr);
    printRV(warnmsg1);
    printRV("\nOR you may continue (ignoring these warnings) --> ");
  }
else
  { /*Mar24/14 - remove (expanded prompt & command k), rely on ?=help */
    /*May27/10 - test switch k0 kill help prompts or k1 restore */
    /* if (helpson) */
    /*{ printf("\nnull=next,r#=rec,s=search,u=update,x=undo,p=print,w=write,e=count"); */
    /*  printf("\n,R#=Recsize,h1=char,h2=auto,g=genseq#,c=chkseq#,t=translate");  */
    /*  printf("\n,q=quit,k0=helpoff,k1=helpon,?=HelpScreens --> "); */
    /*} */
    printf("\nEnter command, or ?=Help, q=quit, null=NextRecord --> ");
  }

/* wait for op command - return point (vs repormpt) if return value 2    */
/* - following transfer search args to write,print,enumerate,drop */
getopcmd:

memset(opcmd,'\0',80);  /* clear any prior command            */
fgets(opcmd,80,stdin);  /* wait for operator reply            */

/* convert any LineFeed in fgets reply to null */
ll = strlen(opcmd);
if ((ll > 0) && (opcmd[ll-1] == '\n'))
  { opcmd[ll-1] = '\0';
  }

ss = parse();                      /* parse oprtr reply                  */
                                   /* validate & store cmd args          */
if (ss <= 0)                       /* command valid status ?             */
    goto reprompt;

/*eject*/
/*-------- analyze operator response & perform indicated action -------- */

if ((opcmd[0] == 'q') || (opcmd[0] == ':') || (opcmd[0] == 0x1B))
  { goto endprgm;
  }

if (memcmp(opcmd,"help",4) == 0)
  { showhelps(hsp,26);
    goto endscreen;
  }

/* test for search cmd 1st, to reset search continue col on any other cmd*/
if (opcmd[0] == 's')                        /* search for pattern ?      */
  { upnext = 1;         /*May25/10 - search will use getrec() to advance */
    /* vrtyp2 = '\0'; */
    /*Sep16/17 - search1 will clear vrtyp2 if new search, but not if ss re-search*/
    ss = search1();
    if (ss <= 0)
      { errmsg("    - reset fileptr to BOF(r0) & retry(ss) ?","","",0,0x00);
        goto reprompt;
      }
    else
      { upnext = 0;     /* inhibit up fileptr/recnum1 to show correct record*/
        goto bgnscreen;
      }
  }

/* reset search continue column# - on any cmd other than search repeat*/
sargs.pscd = 0;                        /* reset search continue col#  */

/* if Null reply - advance to next record, or next screen if rsize>320*/
if (opcmd[0] <= ' ')                   /* null reply ?                */
  { if (rii >= rsz1)
      /* fileptr += rsz1; May25/10 - getrec changed */
      { upnext = 1;     /*May25/10 - set switch to up fileptr in getrec*/
        /*Note - above works for varlth as well as fixlth           */
        /*     - since last getrec stored rsz1 correct for all types*/
        goto bgnrec;
      }
    else
        goto bgnscreen;
  }

/*eject*/
/*Nov12/07 - user enter a cmd (vs ' ' null entry) */
/* clear vrtyp2 in case reading STL Indexed/Relative & next cmd not browse */
/* vrtyp2 = '\0'; */

/* store rsz2 to be used for following fileptr calcs from rec#     */
/* fixed files - use rszo from r optn on cmd line                  */
/* variable files - use last valid recsize (might be prior to EOF) */
if (ftype == 'f')
  { rsz2 = rszo;       /* fixed files - use rszo r optn cmd line   */
  }
else
  { rsz2 = rsz1;       /* varlth files - use current or prior recsize */
    if (rsz2 <= 0)
      { rsz2 = rsz1p;
      }
  }

/*Note - if opcmd # - parse() sets r# for Fixlth & f# for varlth */
if (opcmd[0] == 'r')               /* r# (goto specific record) ? */
  { /* subtract 1 from oprtrs 1 relative entry, verify > 0, recalc ptr*/
    cmn--;
    if (cmn <= 0)
      { cmn = 0;                   /* correct any oprtr confusion */
        recnum1 = 1;               /*Apr19/16 - init 1 for fixlth */
        fileptr = 0;               /* reset fileptr to BOF        */
      }
    else
      { /*Nov11/07 - calc new file ptr & goto display record        */
        /*Jun01/10 - setfileptr() ensures fileptr never > filesize  */
        setfileptr(cmn * rsz2);    /* set ptr, ensure not > filesize*/
        /*Dec07/10 - calc recnum1 in setfileptr if Fixlth           */
      }
    upnext = 0;         /*May25/10 - inhibit fileptr updt in getrec */
    goto bgnrec;
  }

else if (opcmd[0] == 'b')              /* b# goto specific byte#      */
  { setfileptr(cmn);                   /* set ptr, ensure not>filesize*/
    upnext = 0;           /*May25/10 - inhibit fileptr updt in getrec */
    /*Dec07/10 - calc recnum1 in setfileptr if Fixlth           */
    goto bgnrec;
  }

/*eject*/
else if (opcmd[0] == '+')         /* +# (forward #recs)      */
  { upnext = 0;            /*May25/10 - inhibit fileptr updt in getrec */
    if (cmn <= 1)
      { cmn = 1;                  /* default value to 1      */
      }
    /*Nov22/10 - convert +/- cmds to 'f' find for varlth files         */
    if (ftype == 'f')
      { upfileptr(cmn * rsz2);        /* up ptr, ensure not > filesize */
        goto bgnrec; 
      }
    else
      { /*Oct10/16 - handle +1 & +multiple separately, SEE problem descrip below*/
        /* for +1 - just up recnum1++ &  get next record         */
        /* for +multiple - up recnum1 + multiple & use findrec() */
        if (cmn <= 1)
          { /* recnum1++;   I think not, just set upnext = 1 ? */
            upnext = 1;     /* causes getrec to call upfileptr */
            goto bgnrec;
          }
        else
          { /*Nov22/10 - calc rec# for findrec() */
            /*Oct10/16 - clear cargs before insert cargs.cmv & findrec()*/
            cargs = carg0;
            cargs.cmv = (recnum1 + cmn);  /* calc rec# for findrec() */
            ss = findrec();
            if (ss <= 0)
              { goto reprompt;
              }
            goto bgnrec;
          }
      }
  } 

/*Oct10/16 - problem for uvhd STLifile x4y7j4 show all but inhibit data recs  */
/* - using + to advance to next record will bypass records to next index point*/
/* - +1 not finding consecutive sysrecs x20 followed by x10 in ism15000       */
/* - works if not using +1 & page thru the 2048 byte to next sysrec           */
/* - findrec/getrec searches from lower index +1 to next index <=             */
/*   low index 144, next index 264, sysrecs at 260 & 261, 261 bypassed to 264 */
/*Oct10/16 - now handle +1 separately from +multiple SEE above                */

/*eject*/
/*Apr01/07 - calc recnum1 for getrecv/getrecz (+= above & -= below) */
else if (opcmd[0] == '-')               /* -# (back #recs)         */
  { upnext = 0;            /*May25/10 - inhibit fileptr updt in getrec */
    if (cmn <= 1)
      { cmn = 1;                        /* default value to 1      */
      }
    /*Nov22/10 - convert +/- cmds to 'f' find for varlth files */
    if (ftype == 'f')
      { upfileptr(-(cmn * rsz2));     /* up ptr, ensure not > filesize*/
        goto bgnrec; 
      }
    else
      { /*Nov22/10 - calc rec# for find cmd */
        /*Mar13/11 - but if EOF, use (rcount + 1 - cmn) */
        /*Oct10/16 - clear cargs before insert cargs.cmv & findrec()*/
        cargs = carg0;
        cargs.cmv = (recnum1 - cmn);  /* calc rec# for findrec() */
        if (fileptr >= filesize)
          { cargs.cmv = (rcount + 1 - cmn);
          }
        ss = findrec();
        if (ss <= 0)
          { goto reprompt;
          }
        goto bgnrec;
      }
  }

/*eject*/
/*Note - if opcmd # - parse() sets r# for Fixlth & f# for varlth */
/*May24/10 - 'f' find specified rec# for varlth recs (RDW,LST,IDXf3/8) */
else if (opcmd[0] == 'f') 
  { ss = findrec();
    if (ss <= 0)
      { goto reprompt;
      }
    upnext = 0;      /* inhibit up fileptr/recnum1 to show correct record*/
    goto bgnrec;
  }

/*Jun12/07 - optn 'n' multi-recs/screen & command not browse, rec# byte# */
/* - restore fileptr of 1st on screen for print, write, update, etc      */
if (opsi['n'-'a'])
  { fileptr = fileptro1;     /* save fileptr of 1st on screen  */
  }

/*Jun12/07 - 'if else' chgd to 'if' for option 'n' test above (s/b OK) */
if (opcmd[0] == 'p')           /* print formatted recs to a file ? */
  { /*Sep15/17 - getting R/T after Print,Search,etc - try clear vrtyp2 here */
    vrtyp2 = '\0';
    ss = print1(lszo);
    vrtyp2 = '\0';
    if (ss <= 0)
      { errmsg("    - reset fileptr to BOF(r0) & retry(pp) ?","","",0,0x00);
        goto reprompt;
      }
    else if (ss == 2)
      { goto getopcmd;
      }
    else
      { goto bgnrec;
      }
  }

/*Aug15/11 - iprint changed to optin 'i' on print command           */
else if (opcmd[0] == 'i')          /* iprint formatted recs to a file ? */
  { strcpy(opmsg2,
    "iprint changed to option 'i' on 'p' cmd, (p5i print 5 immediately)\n");
    goto bgnrec;
  }

/*eject*/
else if (opcmd[0] == 'w')        /*  write unformatted recs to a file ? */
  { vrtyp2 = '\0';
    ss = write1();
    vrtyp2 = '\0';
    if (ss <= 0)
      { errmsg("    - reset fileptr to BOF(r0) & retry(ww) ?","","",0,0x00);
        goto reprompt;
      }
    else if (ss == 2)
      { goto getopcmd;
      }
    else
      { goto bgnrec;
      }
  }

else if (opcmd[0] == 'd')        /*  copy/drop unformatted recs to a file ? */
  { vrtyp2 = '\0';
    ss = drop1();
    vrtyp2 = '\0';
    if (ss == 2)
      { goto getopcmd;
      }
    goto reprompt;
  }

else if (opcmd[0] == 'c')        /*  check sequence to EOF (or seq err)  */
  { vrtyp2 = '\0';
    ss = check1();
    vrtyp2 = '\0';
    if (ss <= 0)
      { goto reprompt;
      }
    else
      { goto bgnrec;
      }
  }

/*eject*/
else if (opcmd[0] == 'u')                 /* update current record ?   */
  { opstat = update();
    if (opstat <= 0)
        goto reprompt;
    else
        goto bgnrec;
  }
/*Dec06/10 - add move field/constant command */
else if (opcmd[0] == 'm')                 /* move field/constant ? */
  { opstat = move1();
    if (opstat <= 0)
        goto reprompt;
    else
        goto bgnrec;
  }
/*Dec26/10 - add move Numeric command */
else if (opcmd[0] == 'n')                 /* move numeric ? */
  { opstat = moven();
    if (opstat <= 0)
        goto reprompt;
    else
        goto bgnrec;
  }
else if (opcmd[0] == 'v')                 /* scan/replace current record ? */
  { opstat = scanrep();
    if (opstat <= 0)
        goto reprompt;
    else
        goto bgnrec;
  }
/* x = rollback last update, Jun12/07 disallow if option n multi recs/scrn*/
else if ((opcmd[0] == 'x') && (!(opsi['n'-'a'])))
  { opstat = back1();
    if (opstat <= 0)
        goto reprompt;
    else
        goto bgnrec;
  }
/* X = rollback all updates, Jun12/07 disallow if option n multi recs/scrn*/
else if ((opcmd[0] == 'X') && (!(opsi['n'-'a'])))
  { opstat = backall();
    if (opstat <= 0)
        goto reprompt;
    else
        goto bgnrec;
  }

/*eject*/
else if (opcmd[0] == 'g')                 /* sequence number ? */
  { vrtyp2 = '\0';
    opstat = seqgen();
    vrtyp2 = '\0';
    if (opstat <= 0)
        goto reprompt;
    else
        goto bgnrec;
  }
else if (opcmd[0] == 'R')               /* change Rcsz (w/o quit reenter)  */
  { rszo = cmn;                         /* store new rcsz                  */
    chkrcsz(rszo);                      /* validate & calc new recs in file*/
    upnext = 0;                         /* inhibit advance to next record  */
    goto bgnrec;                        /* go redisplay current record     */
  }
else if (opcmd[0] == 'e')               /* enumerate (count) records ?     */
  { vrtyp2 = '\0';
    ss = enum1();
    vrtyp2 = '\0';
    if (ss == 2)
      { goto getopcmd;
      }
    goto bgnrec;                        /* go redisplay current record     */
  }
else if (opcmd[0] == 'a')               /* accumulate fields ?             */
  { vrtyp2 = '\0';
    opstat = acum1();
    vrtyp2 = '\0';
    if (opstat == 2)
      { goto getopcmd;
      }
    goto bgnrec;                        /* go redisplay current record     */
  }
else if (opcmd[0] == 'h')               /* display h0=hex,h1=chars,h2=auto */
  { opsi['h'-'a'] = copsi['h'-'a'];     /* replace cmd line option         */
    upnext = 0;                         /* inhibit advance to next record  */
    goto bgnrec;
  }
else if (opcmd[0] == 'z')               /* truncate filesize           */
  { opstat = trunc1();                  /* execute truncate            */
    goto bgnrec;
  }
else if (opcmd[0] == 't')                 /* translate ? (Aug2004) */
  { opstat = trans();
    if (opstat <= 0)
        goto reprompt;
    else
        goto bgnrec;
  }
else if (opcmd[0] == 'l')     /* close files ? */
  { opstat = close1();
    goto reprompt;
  }

/*eject*/
else if (opcmd[0] == 'k')     /* konvert among binary/packed/numeric/hex ? */
  { opstat = konvert();
    if (opstat == 2)
      { goto getopcmd;
      }
    goto reprompt;            /* go prompt for next command */
    /* goto bgnrec;           |* go redisplay current record */
  }

showhelps(hsp,26);
goto endscreen;

/*eject*/
/*uvhd-------------------- end of program ------------------------*/
endprgm:

/*Mar04/14 - close moved into fileclose() & open/close sizes compared */
fileclose();

/* close output files - if open */
/*Feb05/09 - most output files now opened & closed on each command       */
/* - write has option w1 on cmd line to combine to 1 file - REMOVED 2010 */
/* closefileX(wfname,"W",&wfptr,&wfopn,0x00); */

/*Dec29/10 - cmd 'l' added to close files as desired, so close most here */
/*         - print & drop files always closed at end command             */
/*         - bits 0x00 cause no error if not open                        */
closefileX(wfname,"W",&wfptr,&wfopn,&wfctr,0x00); 
closefileX(pfname,"P",&pfptr,&pfopn,&pfctr,0x00);

printf("** quit request - program ended **\n");

return(0);
/* Feb2002 changed exit(0) to return(0) win lcc complained missing ret val */
}

/*eject*/
/*============================ subrtns =============================*/

/*uvhd---------------------- chkrcsz ---------------------------------*/
/* call subfunctn to check rcsz & calc number of records in file      */
/* - called on program entry (check rcsz entered on cmd line r option)*/
/* - also called by command 'R' (change rcsz w/o quit & cmd line optn */
/* - no errmsgs if variable text of indexed file                      */

int chkrcsz(int recsiz0)
{
/* inhibit errmsgs if variable length records */
/* - option 't' (text) 'v' (IDXf3/IDXF8) 'z' (RDW) */
if ((ftype == 'v') || (ftype == 'z') || (ftype == 't') || (ftype == 'x'))
  { /* calculate items for variable length records */
    /*Nov11/07 - default recsize to 256 if < 1 or > RMAX */
    if (recsiz0 < 1 || recsiz0 > RMAX)
      { recsiz0 = RDFLT;
      }

    /* calc recs in file, etc - based on 256 for text or varlth files */
    /* - will recalc on every record based on current recsize         */
    calcrecs(recsiz0,rszo);          /* calc recs in file, etc */
  }
else
  { /* chkrcsz for fixed length records */
    /*Nov11/07 - remove errmsg if recsize invalid */
    /* - default recsize to option r defltd to 256 at pgm init for fixlth*/
    if (recsiz0 < 1 || recsiz0 > RMAX)
      { recsiz0 = rszo;
      }

    calcrecs(recsiz0,rszo);          /* calc recs in file, etc */

    /* warning if rcsz does not divide evenly into filesize for FixedLth */
    if ((rmndr != 0) && (ftype == 'f'))
      { printf("WARNING: rcsz does not divide evenly into filesize\n");
      }
  }

return(1);
}

/*eject*/
/*uvhd------------------------- calcrecs -----------------------------*/
/*Nov11/07 - calculate records in file, based on current recsize      */
/* - vs prior method calc only at begin program based on optn 'r'     */
/* - if recsize invalid, default to arg2                              */

int calcrecs(int recsiz1, int rsxdflt)
{
/*May27/10 - test switch (set at EOF) inhibit calc rcount for hdgs */
/*Jun03/10 - inhibit for varlth files, allow for Fixed lth         */
if ((rcountx) && (ftype != 'f'))
  { rcount = rcountv;    /* use count from findex varlth files */
    return(0);
  }

/* if recsiz1 <= 0, default to 256 to prevent divide errors */
if (recsiz1 < 1)
  { recsiz1 = rsxdflt;
  }

/* calculate records in file & any remainder              */
frecsc = (filesize / recsiz1);
rcount = frecsc;                /* for printing printf %d */
rmndr = (filesize % recsiz1);
filextra = rmndr;               /* same as rmndr  */
/*Jan10/11 - format remainder into msg for 2nd hdr or null if rmndr 0 */
/*Jan12/11 - do not show rmndr for variable lth files                 */
sprintf(rmndrs," fsize/rsize(rmndr)=%d\n",rmndr);
if ((rmndr == 0) || (ftype != 'f'))
  { rmndrs[1] = '\0';
  }

/* calculate filesize evenly divisible by rcsz (but <)             */
/* - to prevent fileptr getting off record boundaries at EOF -recs */
filesiz0 = frecsc * recsiz1;

return(1);
}

/*eject*/
/*uvhdcob--------------------- upfileptr ---------------------------*/
/*Jun01/10 - up fileptr by record size & ensure never > filesize    */
/*         - if filesize exceeded, return 1, else 0                 */
/*Dec07/10 - calc recnum1 if Fixlth                                 */

int upfileptr(int recsize1)
{
fileptr += recsize1;
if (fileptr > filesize)
  { fileptr = filesize;
    /* return(1); */
  }
if (ftype == 'f')
  { recnum1 = ((fileptr / rszo) +1);
  }
return(0);
}

/*uvhdcob--------------------- setfileptr -------------------------*/
/*Jun01/10 - set fileptr to new value & ensure never > filesize    */
/*         - if filesize exceeded, return 1, else 0                */

int setfileptr(UVi64 newfileptr)
{
fileptr = newfileptr;
if (fileptr >= filesize)
  { /*Apr19/16 - set to last record if request >= filesize */
    fileptr = fileptr1;
    /* return(1); */
  }
if (ftype == 'f')
  { recnum1 = ((fileptr / rszo) +1);
  }
return(0);
}

/*eject*/
/*uvhd------------------------ getrec ------------------------------*/
/* get next record (return record length)                           */
/* - calls subfunction getrecf or getrecv                           */
/* - getrecf used for fixed record size                             */
/* - getrect used for text variable lth to CR/LF option 't'         */
/* - getrecv used for option 'v' IDXFORMAT3 variable indexed        */
/* - getrecx used for option 'x' STL x2=Seqntl,x4=Indexed,x8=Relative*/

int getrec(short bits)
{
int rs1;

/*May19/11 - save fileptr of prior get, used to save fileptr last rec in file*/
fileptrp = fileptr;

/*May25/10 - major changes to getrec(), add following:                   */
/* int rsz1p;      Prior Record Size - saved at end getrec()             */
/* - used to incrmnt fileptr at begin next getrec for varlth (not fixed) */
/* fileptr/recnum1 now incremented at begin getrec()                     */
/* - inhibited if upnext==0 (set 0 at bgn prgm & reposition cmds r#/b#   */
/* - upnext=1 at end getrec() assuming browse follows                    */

/*Jun01/10 - ensure fileptr never > filesize */
if (fileptr >= filesize)
  { fileptr = filesize;     /* set fileptr to filesize - recsize */
    /*Nov21/10 - Index built automatically at startup for varlth files*/
    /* - findex uses 'getrec(1)' vs 'getrec(0)' to inhibit EOF prompt */
    if (bits & 0x01) { ; }
    else 
      { strcpy(opmsg2,
        "EOF, enter -1 to see last record, or 1 return to Begin File\n");
      }

    rcountx = 1;            /*Jun02/10 inhibit subsequent calcrecs     */
    eof1++;                 /* count times reached EOF                 */
    if (eof1 == 1)          /*Jun02/10 - 1st time at EOF ?             */
      { rcount = recnum1;   /*    ifso - store records in file         */
      }
    /*Jun03/10 - at EOF -1 msg shows recnum1 1 less than last record */
    /* - try setting to (recs in file +1), so -1 will calc correctly */
    /*Mar13/11 - disable, rcount 1 too high for varlth RDW files     */
    /* recnum1 = (rcount + 1); */
  }

/*eject*/
/* if fileptr already >= filesize & fileptr < filesize           */
/* - set fileptr back to fileptr1, to redisplay last rec in file */
/* - fileptr1 set to last rec in findex() if varlth or fsize-rsize if fixlth*/
else if (upnext)
  { upfileptr(rsz1p);     /* up by Prior RecSize, ensure not > filesize */
    /* if fileptr now >= filesize                                       */
    /* - set fileptr = (filesize - prior recsize) to redisplay last rec */
    /* - store EOF msg & return to display/prompt                       */
    if (fileptr >= filesize)
      { /* fileptr1 = (filesize - rsz1p); */
        /*May19/11 - fileptr1 set in findex for varlth (RDW,text,IDX) */
        /*         - fileptr1 set at prgm init for fixlth             */
        /*Nov21/10 - Index built automatically at startup for varlth files*/
        /* - findex uses 'getrec(1)' vs 'getrec(0)' to inhibit EOF prompt */
        if (bits & 0x01) { ; }
        else 
          { strcpy(opmsg2,
            "EOF, enter -1 to see last record, or 1 return to Begin File\n");
          }

        rcountx = 1;            /*Jun02/10 inhibit subsequent calcrecs     */
        eof1++;                 /* count times reached EOF                 */
        if (eof1 == 1)          /*Jun02/10 - 1st time at EOF ?             */
          { rcount = recnum1;   /*    ifso - store records in file         */
          }
        /*Jun03/10 - at EOF -1 msg shows recnum1 1 less than last record */
        /* - try setting to (recs in file +1), so -1 will calc correctly */
        /*Mar13/11 - disable, rcount 1 too high for varlth RDW files     */
        /* recnum1 = (rcount + 1); */
        return(0);
      }
    else
      { /*Dec07/10 - incrmnt rec number only if variable length       */
        /*         - fixlth recnum1 calculated in setfileptr/upfileptr*/
        /*Apr19/16 - fix bug STL record count 1 high if Alt Indices   */
        /* - count separately in get subrtns for variable file types  */
        /* if (ftype != 'f') */
        /*   { recnum1++;    */ 
        /*   }               */
      }
  }

rii = 0;                  /* init rec index for getlinr subfunction */

/* clear record area - for text records variable length             */
/* - so search commands wont find data from longer records          */
/* - clear recb to blanks (vs nulls) lth rszo+256 before each getrec */
memset(recb,' ',rcsmax);
/*Jun14/11 - chg rszox to rcsmax = rszo(fixlth) or rdwmax(varlth) */

/*eject*/
/* test file type option & call appropriate subfunction */
if (ftype == 't')
  { rs1 = getrect(bits);
  }
else if (ftype == 'v')
  { rs1 = getrecv(bits);
  }
else if (ftype == 'x')
  { rs1 = getrecx(bits);
  }
else if (ftype == 'z')
  { rs1 = getrdw(bits);
  }
else
  { rs1 = getrecf(bits);
  }

/*May25/10 - set switch to updt fileptr at begin next getrec          */
upnext = 1;    /* set switch to updt fileptr at begin next getrec     */

if (ftype == 'f')
  { calcrecs(rszo,RDFLT);  /* calcrecs based on fixed recsize */
  }
else
  { calcrecs(rs1,RDFLT); /* calcrecs based on latest recsize*/
  }

/*Jun01/10 - save recsize as prior recsize for next getrec/fileptr updt*/
if (rs1 > 0)
  { rsz2p = rsz1p; /*Nov21/10 - save recsize 2 recs back for -1 varlth */
    rsz1p = rs1;   /* save current recsize for next getrec fileptr update*/
  }

/*Oct08/11 - edit fileptr for printf's %ld or %lld for 64/32bit HP gcc */
sprintf(fpedit1,E64,fileptr);

return (rs1);
}

/*eject*/
/*uvhd------------------------ getrecf ------------------------------*/
/* get next record (return record length)                            */
/* - record size from option 'r' on cmd line (default 256)           */
/* - getrecf used for fixed record size                              */

int getrecf(short bits)
{
int ii;
int rs1;
Uchar uc;

fileseek("getrecf");               /* seek to current fileptr position */
rs1 = 0;                           /* reset read size                  */

/*May2001 - fopen,fread,fseek change to open/open64,read,lseek/lseek64*/
rs1 = read(fd1,recb,rszo);      /* get fixed size records           */

/*  remainder = (filesize % rszo); */
/*Mar01/03 - rmndr garbage on print ? should not need to recalc if fixed*/
if (rs1 < 0)
    rs1 = 0;

/* set switch 1 if any bytes < 0x20 or > 0x7E, except \n \r \t \f      */
/* to determine print mode, also depends on option 'h'                 */
/* h2 = auto determine, h1 = force chars only, h0 = hex default  */
/* allow hex chars in last 2 bytes of record for CR/LF at end text recs*/
/* Feb20/03 - scan only to 3rd last byte (dont check for x'0D' or x'0A'*/
xchar = 0;
for (ii=0; ii < rs1-2; ii++)
  { uc = recb[ii];
    if ( uc >= ' ' && uc <= '~')
       continue;
    xchar = 1;
    break;
  }

return(rs1);
}

/*eject*/
/*uvhd------------------------ getrect ------------------------------*/
/* get next record (return record length)                            */
/* - record size from option 'r' on cmd line (default 256)           */
/* - getrect used for text variable lth to LF (option 't')           */
/*May20/10 - change buf read 8000 bytes at begin program       */
/*           and when empty in getcbuf()                       */
/*May22/10 - set bufn=0 each get & set buf rdsize rcszo+128    */
/*         - for reset rec#, need to force reread after seek   */
/*         - ensure rs1 count includes LF,                     */
/*           so fileptr += rcsz OK for fileseek at next getrec */
/*Jan16/15 - allow multi-byte record terminator in hex t=xxxx  */
/* uvhd filename t=FF01  <-- example for EBCDIC End-Of-Record  */

int getrect(short bits)
{
int cc,ii;
int rs1;      /* bytes read until LF found */
int rs2;      /* bytes remaining at EOF when LF missing */
Uchar uc;

fileseek("getrect"); 

rs1 = 0;      /* reset read size                           */
bufn = 0;     /* force buffer fill at begin each text read */ 
eormi = 0;    /* reset index of matched terminator string  */

while (rs1 <= rszoy)
  { cc = getcbuf();           /* get next char via buffered read  */
    if (cc == EOF)
      { /*Mar18/19 - test data remaining at EOF text file (LF missing)*/
        rs2 = (filesize - fileptr);  /* calc data remaining */
        if (rs2 > 0)
          { if (upnext)
              { recnum1++;
              }
            /*Mar18/19 - get ERRmsg on 1st rec in file (good), but rcount is 1 less than file count*/
            /* - 1st time err at EOF in findex(), compensate 1st time switch to add 1 */
            rcountf++;         /* count last record occurrences */
            if (rcountf == 1)
              { rcount++;      /* add 1 on 1st time (findex) */
              }
            sprintf(opmsg1,"ERROR - LineFeed missing on last record (#%d) in text file\n",rcount);
            /*Mar19/19 - endscreen: disaply copies opmsg1 to oppmsg1 for any folwng printrec() */
            return(rs2);
          }
        else
          { return(-1);
          }
      }

    recb[rs1++] = (char) cc; /* append current char on current record */

/*eject*/
    /*Jan16/15 - allow multi-byte record terminator in hex t=xxxx */
    /*         - controlled by eoridx[eormi] */
    /* - eormi match index incremented if current char matches, else reset*/
    /* - match declared if eormi reaches eorlx (length of EORID string)   */
    if (eorlx > 0)           /* t=xxxx string stored ? */
      { if ((Uchar)cc == (Uchar)eoridx[eormi])
          { eormi++;
            if (eormi == eorlx)
              { break;
              }
          }
        else
          { eormi = 0;
          }
      }
    else if (cc == '\n')     /* text rec terminated by LF x'0A' ? */
      { break;
      }
  }

if (rs1 < 0)
  { rs1 = 0;
  }

/* set switch 1 if any bytes < 0x20 or > 0x7E, except \n \r \t \f      */
/* to determine print mode, also depends on option 'h'                 */
/* h2 = auto determine, h1 = force chars only, h0 = hex default  */
/* allow hex chars in last 2 bytes of record for CR/LF at end text recs*/
/* Feb20/03 - scan only to 3rd last byte (dont check for x'0D' or x'0A'*/
xchar = 0;
for (ii=0; ii < rs1-2; ii++)
  { uc = recb[ii];
    if ( uc >= ' ' && uc <= '~')
       continue;
    xchar = 1;
    break;
  }

/*Apr19/16 - fix bug STL record count 1 high if Alt Indices   */
/* - count separately in get subrtns for variable file types  */
if (upnext)
  { recnum1++;        /* count records for text files */
  }
return(rs1);
}

/*eject*/
/*uvhd------------------------- getcbuf ------------------------------*/
/* get 1 byte at a time from a text file (includes LineFeeds)         */
/* - returns 1 character (unsigned) or EOF (-1) at end of file        */
/*May20/10 - change buf read 8000 bytes at begin program & when empty */
/*         - was initing bufn=0 on each getrect & reading only rszo   */
/*May22/10 - set bufn=0 each get & set buf rdsize rcszo+128           */
/*         - so reset rec# works, need to force reread after seek     */

int getcbuf(void)
{
/* static char bufa[16384];        ** buffer read area            */
/* static char *bufp = bufa;       ** buffer pointer to next char */
/* static int bufn = 0;            ** buffer bytes remaining      */
/*May2001 - bufa,bufp,bufn  moved to global storage               */
/*may21/10 - modify getcbuf, clear bufa before read 8000          */

if (bufn == 0)
  { memset(bufa,' ',rszoy);
    bufn = read(fd1,bufa,rszoy);
    bufp = bufa;
  }

bufn--;                         /* decrement bytes remaining */

if (bufn >= 0)
  { return (unsigned char) *bufp++;
  }
else
  { return EOF;
  }
}

/*eject*/
/*uvhd------------------------- getrecv -------------------------------*/
/* get next IDXFORMAT8 length record (Micro Focus COBOL)               */
/*Feb05/09 - combine getrev & old getrecx (IDXFORMAT) as getrecv       */
/* - called from getrec when option 'v' specified                      */
/* - fileptr should point to the 2 or 4 byte hdr of next record        */
/* - if fileptr < file header size, set fileptr = 0                    */
/* - read record header 2 bytes (or 4 bytes if max rcsz >= 4095)       */
/* - calc record size & slot size (hdr+data+fill to 4 byte boundary)   */
/* - reseek to begin rec hdr (to include hdr in display)               */
/* - test rec display optn v1,v2,v4,v8: filehdr, deleted, data, all    */
/*   (default v4 = filehdr + data recs)                                */

int getrecv(short bits)
{
int rs1;                /* record size to be returned              */
char vrhdr[8];          /* work area to read 2 or 4 byte header    */
int ii;
char cc;

/* return point to read next rec if bypassing deleted recs or system recs*/
reread:

/* read record header (2 or 4 bytes) to get record size               */
/* May2001: fseek changed to call fileseek (largefiles lseek/lseek64) */
/*          fread changed to read (since no fseek64)                  */
fileseek("getrecv");
rs1 = read(fd1,vrhdr,vrhs);        /* get record headr 2 or 4 bytes   */
if (rs1 <= 0)
  { return (rs1);
  }
switchx(vrhdr,vrhs,vrup48,0x00);  /* calc & store rcsz,slotsz,& rectype*/
/* switchx stores vrsize & vrslot & applies max & min rules           */
/* switchx return value used only for min & max at file open          */
/* reseek to begin rec hdr & read record to be displayed slot size    */
/* May2001: fseek changed to call fileseek (largefiles lseek/lseek64) */
fileseek("getrecv");
rs1 = read(fd1,recb,vrslot);      /* get rechdr + data + any fill     */
if (rs1 <= 0)
  { return (rs1);
  }
/* verify record type x'10' thru x'80'                              */
/* - if invalid we must have gone 'off the rails'                   */
/* - 'off the rails' displays partrecs or rereads til EOF reached   */
/* - try scanning to next data type record header x'40' 1st nibble  */
if ((vrtyp < 0x10) || (vrtyp > 0x80))
  { for (ii = vrhs; ii < vrslot; ii++)
      { cc = recb[ii] & 0xF0;     /* isolate 1st nibble             */
        if (cc == 0x40)           /* if possible data rec           */
          { fileptr += ii;        /* set fileptr to possible data rec*/
            goto reread;
          }
      }
  }

/*eject*/
/* test rec display optn v1,v2,v4,v8: filehdr, deleted, data, all    */
if ((vrtyp == 0x30) && (opsi['v'-'a'] & 0x01) && (fileptr < vfhs))
  { if (upnext) { recnum1++; }
    goto process;
  }
else if (vrtyp == 0x20)
  { /*Oct08/16 - add deleted count to displays & Indexed file save/load */
    dcount1++;
    if (opsi['v'-'a'] & 0x02)
      { if (upnext) { recnum1++; }
        goto process;
      }
    else
      { fileptr += rs1;
        goto reread;
      }
  }
else if ((vrtyp == 0x40) && (opsi['v'-'a'] & 0x04))
  { /*Apr19/16 - fix bug STL record count 1 high if Alt Indices   */
    /* - count separately in get subrtns for variable file types  */
    if (upnext) { recnum1++; }
    goto process;
  }
/* sysrec (not filehdr) or other record - bypass unless option v8 (all) */
else if (opsi['v'-'a'] & 0x08)
  { if (upnext) { recnum1++; }
    goto process;
  }
else
  { /* current record not desired - bypass, return to get next         */
    fileptr += rs1;
    goto reread;
  }

/* record selected for process/display by calling mainline         */
process:
xchar = 1;      /* set switch to display record in hexadecimal   */

return (rs1);
}

/*eject*/
/*----------------------------- getrecx -------------------------------*/
/*Feb20/16 - get record for STL files                                  */

int getrecx(short bits)
{
int rs1;          /* record size to be returned                   */
char vrhdr[16];   /* work area to read record header 4 or 8 bytes */
char vrhdrx[16];  /* vrhdr in hex for R/T errmsg */
char vrhdr2[16];  /* read unused space at end data */
int ii;
char cc;
xchar = 1;        /* set switch to display record in hexadecimal   */

/* separate read's depending on Sequential, Indexed,or Relative */
if (opsi['x'-'a'] & 0x02)
  { rereads:                 /* return point if file hdr record */
    memset(recb,' ',4096);   /* clear record area */
    fileseek("get STL seqntl hdr");
    /* if fileptr 0 - bypass file hdr record, 112 bytes seqntl */
    if (fileptr == 0)
      { rs1 = read(fd1,vfhdr,112);  /* bypass file hdr Seqntl */
        /*Oct09/16 - option y1 show hdr rec STLS as well as STLI&STLR*/
        if (opsi['y'-'a'] & 0x01)
          { if (upnext)
              { recnum1++;          /* up rec# for filehdr recs ? */
              }
            memcpy(recb,vfhdr,112); /* copy hdrrec for display */
            return(rs1);
          }
        else
          { fileptr += rs1;
            goto rereads; 
          }
      }
    else
      { rs1 = read(fd1,vrhdr,4);  /* get rechdr 4 bytes for STLS */
        if (rs1 <= 0)
          { return (rs1);
          }
        switchx(vrhdr,4,0,0x01);  /* calc&store rcsz,slotsz,& rectype*/
        /* int switchx(char *vhdr, int vrhs, int vrup48, short bits) */
        /* switchx 4 args, arg2 hdrsiz, arg3 roundup4/8, arg4 inhib +hdrsiz*/
        /* for STLS - read 8 bytes extra for hdrsize & trailing hdrsize */
        /*          - switchx arg2 0x02 inhibited add hdrsize to slotsize */
        vrslot += 8;                   /* +8 for recsize before & after   */
        fileseek("get STL seqntl hdr+data");
        rs1 = read(fd1,recb,vrslot);   /* get rechdr + data +4 +any fill  */
        if (rs1 <= 0)
          { return (rs1);
          }
        /*Apr19/16 - fix bug STL record count 1 high if Alt Indices   */
        /* - count separately in get subrtns for variable file types  */
        if (upnext)
          { recnum1++;        /* count records for STLs files */
          }
        return (rs1);
      }
  }

/*eject*/
else if (opsi['x'-'a'] & 0x04)
  { /* process STL Indexed file records */
    /* Indexed record types: 0x00=filehdr,0x10/0x20=system,0x40=data,0x80=deleted*/
    /* 4 byte hdr: 1 type + 3 byte data size, except system recs */
    /*Mar30/18 - STL 5.1 system recsize now 32K/64K in hdr 140(4) (vs 4K assumed)*/
    rereadi:   /* return point if file hdr or not valid data rec*/
    fileseek("get STL Indexed record");

    /*Sep16/17 - try clear vrtyp2 if (upnext == 0) */
    if (!(upnext))
      { vrtyp2 = '\0';
      }

    /*Aug28/17 - test vrtyp2 from prior readhdr/switchx for unused space at end data*/
    /* vrtyp2 & 0x08 = unused space at end data record exists         */
    /*                 unused space value is in 4 bytes at end data   */
    /* vrtyp2 & 0x0D = unused space is 1,2,or 3 bytes (in low 2 bits) */
    if (vrtyp2 & 0x08) 
      { if ((vrtyp2 & 0x01) && (vrtyp2 & 0x02))
          { fileptr +=3;
          }
        else if (vrtyp2 & 0x02)
          { fileptr +=2;
          }
        else if (vrtyp2 & 0x01)
          { fileptr +=1;
          }
        else
          { rs1 = read(fd1,vrhdr2,4);
            if (rs1 <= 0)
              { return(rs1);
              }
            switchx(vrhdr2,4,0,0x01);    /* calc value in 4 bytes at end data */
            fileptr += vrsize;           /*Sep15/17 - up actual (can't assume 4) */
          }
      }

    /* vrtyp2 = '\0'; disable, redundant, switchx below re-stores */
    /*Sep15/17 - getting R/T err, extra not bypassed after Print, Search, etc*/

    fileseek("get STL Indexed hdr after check unused space");
    rs1 = read(fd1,vrhdr,4);  /* get rechdr 4 bytes for Indexed */
    if (rs1 <= 0)
      { return (rs1);
      }
    switchx(vrhdr,4,0,0x01); /* calc&store rcsz,slotsz,& rectype*/

/*eject*/
    /*Oct07/16 - read data using size in hdr, except system recs */
    /* - using slot size vrslot, calculated by switchx from vrhdr*/
    /* - rereads hdr + data in the data record area (vs record hdr area) */
    if ((vrtyp == 0x20) || (vrtyp == 0x10))
      { vrslot = vrsys;      /* override vrhdr slot size for system recs     */
        /*Mar30/18 - STL 5.1 sysrecsize 140(4) in hdr 32K/64K (vs 4K assumed)*/
      }
    memset(recb,' ',4096);            /* clear record area */
    fileseek("get STL Indexed data");
    rs1 = read(fd1,recb,vrslot);      /* get rechdr + data +any fill  */
    if (rs1 <= 0)
      { return (rs1);
      }

    /* process Indexed record depending on record type */
    /* 0x00=filehdr, 0x10/0x20=system, 0x40=data, 0x80=deleted, other=errmsg */
    /* allow filehdr only at begin file (fileptr=0) */
    if ((vrtyp == 0x00) && (fileptr == 0))
      { /* test option y1 to show filehdr records */
        if (opsi['y'-'a'] & 0x01)
          { if (upnext)
              { recnum1++;          /* up rec# for filehdr recs ? */
              }
            return(rs1);
          }
        else
          { fileptr += rs1;
            goto rereadi; 
          }
      }

    else if (vrtyp == 0x40)
      { /* process Indexed data record type=0x40 */
        /*Apr19/16 - fix bug STL record count 1 high if Alt Indices   */
        /* - count separately in get subrtns for variable file types  */
        if (upnext)
          { recnum1++;        /* count records for STLi files */
          }

        /*Oct06/16 - option j4 to inhibit showing valid data records */
        if ((opsi['j'-'a'] & 0x04) && (!IndexBuild))
          { fileptr += rs1;
            goto rereadi; 
          }
        return(rs1);
      }

/*eject*/
    else if (vrtyp == 0x80)  /* Deleted record ? */
      { /* process Indexed deleted records */
        /*Dec06/16 - dont count deleted system recs x'8000100000000000' */
        if (memcmp(recb,"\x80\x00\x10\x00\x00\x00\x00\x00",8))
          { dcount1++;
          }
        if (opsi['y'-'a'] & 0x04)
          { if (upnext)
              { recnum1++;          /* up rec# for deleted recs ? */
              }
            return(rs1);
          }
        else
          { fileptr += rs1;
            goto rereadi; 
          }
      }

    else if ((vrtyp == 0x20) || (vrtyp == 0x10))
      { /* process system records */
        /*Mar30/18 - STL 5.1 sysrecsize 140(4) in hdr 32K/64K (vs 4K assumed)*/
        if (opsi['y'-'a'] & 0x02)
          { if (upnext)
              { recnum1++;          /* up rec# for system recs ? */
              }
            return(rs1);
          }
        else
          { fileptr += rs1;
            goto rereadi; 
          }
      }

    else
      { /*Oct07/16 - add invalid R/T msgs for STL Indexed files */
        printf("STL Indexed R/T invalid, not x00=filehdr,x40=data,x80=deleted,x20/x10=system\n");
        data2hex(vrhdrx,vrhdr,8,0x02);
        printf("fptr=%ld, rec#=%d, rechdr=%s\n",fileptr,recnum1,vrhdrx);
        printf("enter to continue, will show bad record next (unless building index)\n");
        fgets(opcmd,80,stdin);
        if (upnext)
          { recnum1++;
          }
      }
  }

/*eject*/
else if (opsi['x'-'a'] & 0x08)
  { /* process STL Relative file records */
    /* relative record types: 0x00=filehdr,0x10/0x20=system,0x40=data,0x80=deleted*/
    /* 4 byte hdr: 1 type + 3 byte data size, except system recs         */
    /*Mar30/18 - STL 5.1 sysrecsize 140(4) in hdr 32K/64K (vs 4K assumed)*/
    rereadr:   /* return point if file hdr or not valid data rec*/
    fileseek("get STL Relative record");

    /*Sep16/17 - try clear vrtyp2 if (upnext == 0) */
    if (!(upnext))
      { vrtyp2 = '\0';
      }

    /*Aug28/17 - test vrtyp2 from prior readhdr/switchx for unused space at end data*/
    /* vrtyp2 & 0x08 = unused space at end data record exists         */
    /*                 unused space value is in 4 bytes at end data   */
    /* vrtyp2 & 0x0D = unused space is 1,2,or 3 bytes (in low 2 bits) */
    if (vrtyp2 & 0x08) 
      { if ((vrtyp2 & 0x01) && (vrtyp2 & 0x02))
          { fileptr +=3;
          }
        else if (vrtyp2 & 0x02)
          { fileptr +=2;
          }
        else if (vrtyp2 & 0x01)
          { fileptr +=1;
          }
        else
          { rs1 = read(fd1,vrhdr2,4);
            if (rs1 <= 0)
              { return(rs1);
              }
            switchx(vrhdr2,4,0,0x01);    /* calc value in 4 bytes at end data */
            fileptr += vrsize;           /*Sep15/17 - up actual (can't assume 4) */
          }
      }

    /* vrtyp2 = '\0'; disable, redundant, switchx below re-stores */
    /*Sep15/17 - getting R/T err, extra not bypassed after Print, Search, etc*/
    fileseek("get STL Relative hdr after check unused space");
    rs1 = read(fd1,vrhdr,4);  /* get rechdr 4 bytes for Relative */
    if (rs1 <= 0)
      { return (rs1);
      }
    switchx(vrhdr,4,0,0x01); /* calc&store rcsz,slotsz,& rectype*/

    /*Oct07/16 - read data using size in hdr, except system rec   */
    /* - using slot size vrslot, calculated by switchx from vrhdr */
    /* - rereads hdr + data in data record area (vs rechdr area)  */
    if ((vrtyp == 0x20) || (vrtyp == 0x10))
      { vrslot = vrsys;   /* override vrhdr slot size for sysrecs */
      }
    memset(recb,' ',4096);            /* clear record area */
    fileseek("get STL Relative data");
    rs1 = read(fd1,recb,vrslot);      /* get rechdr + data +any fill  */
    if (rs1 <= 0)
      { return (rs1);
      }
/*eject*/
    /* process Relative record depending on record type */
    /* 0x00=filehdr, 0x10/0x20=system, 0x40=data, 0x80=deleted, other=errmsg */
    /* allow filehdr only at begin file (fileptr=0) */
    if ((vrtyp == 0x00) && (fileptr == 0))
      { /* test option y1 to show filehdr records */
        if (opsi['y'-'a'] & 0x01)
          { if (upnext)
              { recnum1++;          /* up rec# for filehdr recs ? */
              }
            return(rs1);
          }
        else
          { fileptr += rs1;
            goto rereadr; 
          }
      }

    else if (vrtyp == 0x40)
      { /* process Relative data record type=0x40 */
        /*Apr19/16 - fix bug STL record count 1 high if Alt Indices   */
        /* - count separately in get subrtns for variable file types  */
        if (upnext)
          { recnum1++;        /* count records for STLi files */
          }

        /*Oct06/16 - option j4 to inhibit showing valid data records */
        if ((opsi['j'-'a'] & 0x04) && (!IndexBuild))
          { fileptr += rs1;
            goto rereadr; 
          }
        return(rs1);
      }

/*eject*/
    else if (vrtyp == 0x80)  /* Deleted record ? */
      { /* process Relative deleted records */
        /*Dec06/16 - dont count deleted system recs x'8000100000000000' */
        if (memcmp(recb,"\x80\x00\x10\x00\x00\x00\x00\x00",8))
          { dcount1++;
          }
        if (opsi['y'-'a'] & 0x04)
          { if (upnext)
              { recnum1++;          /* up rec# for deleted recs ? */
              }
            return(rs1);
          }
        else
          { fileptr += rs1;
            goto rereadr; 
          }
      }

    else if ((vrtyp == 0x20) || (vrtyp == 0x10))
      { /* process system records */
        /*Mar30/18 - STL 5.1 sysrecsize 140(4) in hdr 32K/64K (vs 4K assumed)*/
        if (opsi['y'-'a'] & 0x02)
          { if (upnext)
              { recnum1++;          /* up rec# for system recs ? */
              }
            return(rs1);
          }
        else
          { fileptr += rs1;
            goto rereadr; 
          }
      }

    else
      { /*Oct07/16 - add invalid R/T msgs for STL Relative files */
        printf("STL Relative R/T invalid, not x00=filehdr,x40=data,x80=deleted,x20/x10=system\n");
        data2hex(vrhdrx,vrhdr,8,0x02);
        printf("fptr=%ld, rec#=%d, rechdr=%s\n",fileptr,recnum1,vrhdrx);
        printf("enter to continue, will show bad record next (unless building index)\n");
        fgets(opcmd,80,stdin);
        if (upnext)
          { recnum1++;
          }
      }
  }

return (rs1);
}

/*eject*/
/*--------------------------- getrdw --------------------------------*/
/*Apr27/11 - get BDW/RDW record (code rewriten April 27/2011)        */
/* - common to initial build Index & mainline program get records    */
/* - build Index calculated bdmin to assist BDW bypass on get/search */

int getrdw(short bits)
{
int rs1;
xchar = 1;     /* force hex display for RDW files */

/* process depending on initial building Index OR get/search for data records*/
if (fii1 < 2)
  { /* collect stats, calc bdwmin to assist bypass BDW on get/search   */
    /* - can read headers only, OMIT reading data records to save time */
    rdwsize = getrdwh(0);       /* get BDW/RDW header (getrdwh 1 of 3) */
    if (rdwsize <= 0)
      { return(0);
      }

    /* process depending on z2,z4,z8 */
    if (opsi['z'-'a'] & 0x02)
      { rdwsize += 2;         /* z2 hdrsize NOT included in RDWsize */
        /* fileptr += rdwsize; <--Apr27/11 fileptr incrmnt in getrec() */
      }
    else if (opsi['z'-'a'] & 0x04)
      { ;
        /* fileptr += rdwsize; <--Apr27/11 fileptr incrmnt in getrec() */
      }

/*eject*/
    else 
      { /* z8 BDW - special processing if initial build index */
        /* collect stats, calc bdwmin to assist bypass BDW on get/search*/
        if ((fileptr == 0) || (fileptr == bdwnext))
          { bdwsize = rdwsize;
            bdwnext = (fileptr + bdwsize);
            bdwsum += bdwsize;
            bdwcnt++;              /* count BDWs to calc avg at EOF */
            /*May04/11 - save info for rdwerr() reporting */
            bdwfp   = fileptr;
            bdwrn   = recnum1;
            memcpy(bdwsave,rdwhdr,8);
            /* save BDW min/max */
            if (bdwmin == 0)
              { bdwmin = bdwsize;
              }
      
            /*Jun12/06 - ensure BDWmin not saved from last block in file */
            /* if ((bdwmin > bdwsize) && (fileptr < filesizeb)) */
            /*Jun15/06 - fix Jun12 save bdwmin if > bdwsize & bdwmin > rdwmax */
            if ((bdwmin > bdwsize) && (bdwmin > rdwmax))
              { bdwmin = bdwsize;
              }
            if (bdwmax < bdwsize)
              { bdwmax = bdwsize;
              }

            rdwsize = getrdwh2(0);     /* convert RDW to binary */
            /* - getrdwh2 uses bytes 5-8 already read by getrdwh above*/
        
            if (rdwsize <= 0)
              { return(0);
              }
            if (rdwsize > bdwsize)
              { rdwerr("ERR: RDWsize > BDWsize (on Index build phase)",0x20);
              }
            fileptr += 4;            /* bypass BDW */
          }
        else
          { /* z8 BDW on build index, BUT not at BDW */
            /* - up file ptr by size of current record */
            /* - can OMIT reading data on Index build to save time */
            ;
            /* fileptr += rdwsize; <--Apr27/11 fileptr incrmnt in getrec() */
          }
       }

/*eject*/
    rdwsum += rdwsize;
    if (rdwmin == 0)
      { rdwmin = rdwsize;
      }
    if (rdwmin > rdwsize)
      { rdwmin = rdwsize;
      }
    if (rdwmax < rdwsize)
      { rdwmax = rdwsize;
      }
    /*Apr19/16 - fix bug STL record count 1 high if Alt Indices   */
    /* - count separately in get subrtns for variable file types  */
    if (upnext)
      { recnum1++;        /* count records for RDW files */
      }
    return(rdwsize);
  }

/*eject*/
else
  { /* get or find search for DATA RECORDS (vs INITIAL BUILD INDEX)    */
    rdwsizep = rdwsize;         /* save prior RDW size                 */
    rdwsize = getrdwh(0);       /* get BDW/RDW header (getrdwh 2 of 3) */
    if (rdwsize <= 0)
      { return(0);
      }

    fileseek("getrdw");    /* set fileptr from fileptr long int */

    /* process depending on z2,z4,z8 */
    if (opsi['z'-'a'] & 0x02)
      { rdwsize += 2;                 /* z2 hdrsize NOT included in RDWsize*/
        rs1 = read(fd1,recb,rdwsize);
      }
    else if (opsi['z'-'a'] & 0x04)
      { rs1 = read(fd1,recb,rdwsize); /* z4/z8 hdrsize INCLUDED in RDWsize */
      }
    else
      { /* z8 BDW NOT building Index - get data record           */
        /* bypass BDW, identified by:                            */
        /* - bdwsize >= bdwmin (caluclated on index build phase) */
        /* - followed by RDW < BDW & 7th/8th bytes null          */
        if ((rdwsize >= bdwmin) && (rdwhdr[6] == '\0') && (rdwhdr[7] == '\0'))
          { /*May04/11 - save info for rdwerr() reporting */
            bdwfp   = fileptr;
            bdwrn   = recnum1;
            memcpy(bdwsave,rdwhdr,8);
            bdwsizep = bdwsize;        /* save prior BDW size   */
            bdwsize = rdwsize;         /* save current BDW size */
            fileptr += 4;              /* bypass BDW            */
            rdwsize = getrdwh(0);      /* get BDW/RDW header (getrdwh 1 of 3) */
            if (rdwsize <= 0)
              { return(0);
              }
          }
        /* get RDW record (either 1st in BDW or subsequent) */
        fileseek("getrdw"); 
        rs1 = read(fd1,recb,rdwsize);
        if (rdwsize <= 0)
          { return(0);
          }
      }
    /*Apr19/16 - fix bug STL record count 1 high if Alt Indices   */
    /* - count separately in get subrtns for variable file types  */
    if (upnext)
      { recnum1++;        /* count records for RDW files */
      }
  }
return(rs1);
}

/*eject*/
/*uvhd------------------------- getrdwh -------------------------------*/
/* get RDW/BDW header (8 bytes)                                        */
/* - variable length RDW option z verified z2,z4,or z8                 */
/* - called from getrec when option 'z' specified                      */
/* - fileptr should point to the header/prefix of next record          */
/* - read record header 4 bytes & store recsize/slotsize               */
/* - recsize in hdr includes 4 bytes + data size                       */
/* - reseek to begin rec hdr (to include hdr in display)               */
/*Apr26/11 - rewritten to improve BDW detect/bypass                    */

int getrdwh(short bits)
{
int rs1;        /* recsize from read(fd1,rdwhdr,40)       */
int rs2;        /* recsize to return - depending optn z16 */

/*Mar04/2022 - option z16 for hdrsize = TOTAL recsize (hdr+data)    */
/*           - vs default hdrsize = just datasize excluding hdrsize */

fileseek("getrdw");
rs1 = read(fd1,rdwhdr,40); /*Apr27/10 - get 40 bytes for rdwerr() */

/*Mar05/11 - add EOF test here in getrdw1 as well as getrdw */
if (rs1 <= 0)
  { return (rs1);
  }

/* verify BDW/RDW z8/z4 prefix - bytes 3 & 4 must be nulls */
if (opsi['z'-'a'] & 0x0C) 
  { if ((rdwhdr[2] == '\0') && (rdwhdr[3] == '\0'))
      { ; }
    else
      { rdwerr("Invalid BDW/RDW prefix (bytes 3 & 4 not null)",0x20);
      }
  }

/* if BDW(z8) at begin file - verify 1st RDW (bytes 7&8 null */
if ((fileptr == 0) && (opsi['z'-'a'] & 0x08))
  { if ((rdwhdr[6] == '\0') && (rdwhdr[7] == '\0'))
      { ; }
    else
      { rdwerr("z8 at begin file, bytes 7 & 8 not null, try z4",0x20);
      }
  }

/*eject*/
/* convert the BDW/RDW recsize to binary            */
/* z2=2bytehdr, z4=4bytehdr, z8=BDW/RDW 4+4         */
/* - assume big-end unless option z1 for little-end */
memset(u1.cc,'\0',4);            /* clear work area union      */
if (opsi['z'-'a'] & 0x01)
  { memcpy(u1.cc,rdwhdr,2);      /* store little-end recsize on left*/
    /* switch little-end to Big-end if AIX,HP,SUN,etc (vs Intel)    */
#if (BEM)
    u2.cc[0] = u1.cc[3];
    u2.cc[1] = u1.cc[2];
    u2.cc[2] = u1.cc[1];
    u2.cc[3] = u1.cc[0];
#else
    memcpy(u2.cc,u1.cc,4);
#endif
  }
else
  { memcpy(u1.cc+2,rdwhdr,2);    /* store big-end recsize on right*/
/* switch big-end to little-end if INTEL or SCO (not SUN, HP, etc)*/
#if (LEM)
    u2.cc[0] = u1.cc[3];
    u2.cc[1] = u1.cc[2];
    u2.cc[2] = u1.cc[1];
    u2.cc[3] = u1.cc[0];
#else
    memcpy(u2.cc,u1.cc,4);
#endif
  }
/* verify BDW/RDW prefix size */
if ((u2.ii < 8) || (u2.ii > 32768))
  { rdwerr("Invalid BDW/RDW recsize < 8 or > 32768",0x20);
  }

/*Mar03/2022 - add option z16 RDW files hdrsize=total-recsize vs datasize     */
/* z16 - subtract hdrsize in getrdwh, existing logic assumes hdrsize=datasize */
rs2 = u2.ii;                        /* simplify coding below */
if (opsi['z'-'a'] & 0x10)           /* z16 hdrsize=total (vs data only) */
  { if (opsi['z'-'a'] & 0x02)       /* z2  hdrsize = 2 */
      { rs2 = (rs2 - 2);
      }
    else if (opsi['z'-'a'] & 0x04)  /* z4  hdrsize = 4 */
      { rs2 = (rs2 - 4);
      }
  }

return (rs2);
}

/*eject*/
/*---------------------------- getrdwh2 ---------------------------------*/
/*Apr27/11 - convert RDW recsize to binary depending big-end/little-end  */
/* - getrdwh has read 40 bytes & proven BDW in 1st 4                     */
/* - getrswh2 called only for BDW files                                  */
/* - now convert RDW recsize to binary from bytes 5-8 (no need to reread)*/

int getrdwh2(short bits)
{
int rs1;              /* record size to be returned */

/* convert the RDW recsize to binary                */
/* - assume big-end unless option z1 for little-end */
memset(u1.cc,'\0',4);            /* clear work area union       */
if (opsi['z'-'a'] & 0x01)
  { memcpy(u1.cc,rdwhdr+4,2);    /* recsize bytes 5&6 to left   */
    /* switch little-end to Big-end if AIX,HP,SUN,etc (vs Intel)*/
#if (BEM)
    u2.cc[0] = u1.cc[3];
    u2.cc[1] = u1.cc[2];
    u2.cc[2] = u1.cc[1];
    u2.cc[3] = u1.cc[0];
#else
    memcpy(u2.cc,u1.cc,4);
#endif
  }
else
  { memcpy(u1.cc+2,rdwhdr+4,2);    /* recsize bytes 5&6 to right  */
/* switch big-end to little-end if INTEL or SCO (not SUN, HP, etc)  */
#if (LEM)
    u2.cc[0] = u1.cc[3];
    u2.cc[1] = u1.cc[2];
    u2.cc[2] = u1.cc[1];
    u2.cc[3] = u1.cc[0];
#else
    memcpy(u2.cc,u1.cc,4);
#endif
  }

/* verify BDW/RDW prefix size */
if ((u2.ii < 8) || (u2.ii > 32768))
  { rdwerr("Invalid RDW recsize < 8 or > 32768",0x20);
  }

return (u2.ii);
}

/*eject*/
/*------------------------------ rdwerr -------------------------------*/
/*Apr27/11 - report BDW/RDW record prefix error                        */
/* - display callers errmsg + recnumber + fileptr                      */
/* - display 1st 20 bytes of record (read by getrdwh)                  */

int rdwerr(char *msg, short bits)
{
printf("%s\n",msg);
printf("ERR in RDW=%d at rec#=%d, fileptr=%ld, filesize=%ld\n",
        rdwsize, recnum1, fileptr, filesize);
if (opsi['z'-'a'] & 0x08)
  { printf("prior BDW=%d saved from rec#=%d, fileptr=%ld\n",
        bdwsize, bdwrn, bdwfp);
    data2hex(bdwsavehex,bdwsave,16,0x02);
    printf("  prior BDW/RDW(hex)=%s\n",bdwsavehex);
  }

/*Jun12/11 NOTE - rdwsize/bdwsize reported could be prior vs current */

/*May07/11 - show current BDW/RDW in horizontal hex (match prior BDW) */
data2hex(bdwcurhex,rdwhdr,16,0x02);
printf("current BDW/RDW(hex)=%s\n",bdwcurhex);

printf("1st 40 bytes shown below in vertical hexadecimal\n");
data2vhx(w1,w2,w3,rdwhdr,40,ebc2asc,vhxbits);
printf("%s\n",w1);
printf("%s\n",w2);
printf("%s\n",w3);
exit(99);
return(0);
}

/*eject*/
/*uvhd------------------------- getlinr ------------------------------*/
/* get data for next display line (from record buffer)                */
/* - 64 bytes max, or 'lszo' if option 'l' spcfd                      */
/* - or less at EOR or EOF                                            */
/* - return number of bytes copied or 0 at EOR/EOF                    */

int getlinr(int lsize)
{
lii = 0;                               /* reset line index            */

rii1 = rii;                            /* save rec index b4 new segment*/

if (rii >= rsz1)                       /* EOR/EOF already reached ?   */
    return(0);

while ((lii < lsize) && (rii < rsz1))
  { linr[lii++] = recb[rii++];
  }

return(lii);
}

/*eject*/
/*uvhd------------------------filterchar-------------------------------*/
/* subrtn to convert non-display characters to periods                 */
/*Apr04/06 - changed unprintable char rep from '.' period to '^' carat */
/*Apr04/06 - dont like it change it back                               */
/*Dec16/20 - add option t1,2,4,8 tchar replace unprints on hex charline*/
/*         - t1 '.' (default), t2 ':', t4 '^', t8 '_'                  */
void filterchar(unsigned char *ch, int lsize)
{
int i;
for (i=0; i < lsize; i++)
  {  if ((ch[i] < ' ') || ((ch[i] > 0x7e) && ((opsi['c'-'a'] & 0x01) == 0)))
         ch[i] = tchar; /* replace unprints t1=.(default),t2=:,t4=^,t8=_ */
  }
}
/*Feb2003 - LMAX changed to lsize when LMAX increased to 1024      */
/*        - lsize now passed as argument (for screen & print diff) */
/*        - in filterchar, hexzone,& hexdigit                      */

/*uvhd----------------------- hexzone ------------------------------*/
/* convert zones of char string to hex display                      */
void hexzone(char *hexzo, char *hexch, int lsize)
{
intU z;
int i;
for (i=0; i < lsize; i++)
  { z = (intU)(hexch[i] >> 4);
    z = z & 0x0f;
    if (z < 10)
        hexzo[i] = (char)(z + 0x30);
    else
        hexzo[i] = (char)(z + 0x37);
  }
hexzo[i] = '\0';
}

/*uvhd---------------------- hexdigit ------------------------------*/
/* convert ddigits of char string for hex display                   */
void hexdigit(char *hd,char *hc, int lsize)
{
intU d;
int i;
for (i=0; i < lsize; i++)
  { d = (intU)(hc[i] & 0x0f);
    if (d < 10)
        hd[i] = (char)(d + 0x30);
    else
        hd[i] = (char)(d + 0x37);
  }
hd[i] = '\0';
}

/*eject*/
/*uvhd--------------------------- boldpat ----------------------------------*/
/* highlight last search patterns if in current line segment to be displayed*/
/*May2003 - enhanced to allow 3 search patterns to be highlighted           */
/*        - by using table of dsplcmnts to start & end highlighting         */

short boldpat(Uchar *linp, Uchar *linc)
{
int lincl;          /* length of input data                         */
int lincde;         /* dsplcmnt in record to end of current segment */
int p1d,p1l,p1de;   /* dsp,lth,end of 1st search pattern            */
int p2d,p2l,p2de;   /* dsp,lth,end of 2nd search pattern            */
int p3d,p3l,p3de;   /* dsp,lth,end of 3rd search pattern            */
int match;          /* match indicator considering 3 paterns AND/ORs*/
int ii,jj,tt;       /* indices for I/O data & table of start/stop dsps*/
int bt[8];          /* table of start/end dsplcmnts to be bolded    */
                    /* - will be sorted by start dsplcmnt           */
                    /* - unused entries disabled by dsp=262144      */

lincl = strlen((char*)linc);            /* calc length of input record */

/* copy input to output, presuming no search/no match in current segment*/
strcpy((char*)linp,(char*)linc);

/*Feb23/14 - inhibit re-search if highlighting only */
if (hil1l > 0)
  { /*Mar24/14 - inhibit high-lighting if DOS (causes garbage) */
#ifdef DWIN
    return(0);
#endif
    p1d = hil1d;
    p1l = hil1l;
    p2d = hil2d;
    p2l = hil2l;
    p3d = hil3d;
    p3l = hil3l;
  }
/*eject*/
else
  { /* bypass if inhibited via option i1 or if no search pattern stored    */
    /*Jan10/11 - option chg to i from b now used for Begin display fileptr */
    if ((opsi['i'-'a'] & 0x01) || (sargs.a2l == 0))
      { return(0);
      }

    /* re-search to ensure current record contains data of last search    */
    p1d = search2(recb,&sargs);
    p1l = sargs.a2l;
    
    if (sargs.a4l)               /* if 2nd search pattern specified ?     */
      { p2d = search3(recb,&sargs);
        p2l = sargs.a4l;
      }
    else
      { p2d = 262144;             /* force match for 2nd pattern omitted    */
        p2l = 0;                 /* disable pattern 2                       */
      }                          /* 262144 inhibits BOLD when no op4 pattern*/
    
    if (sargs.a6l)               /* if 3rd search pattern specified ?      */
      { p3d = search4(recb,&sargs);
        p3l = sargs.a6l;
      }
    else
      { p3d = 262144;             /* force match for 2nd pattern omitted    */
        p3l = 0;                 /* disable pattern 3                       */
      }                          /* 262144 inhibits BOLD when no op4 pattern*/
    
    /* bypass if search conditions not met                               */
    /* test results of 3 searches depending on conditions vs AND/ORs possible*/
    /*May2003 - op4/op5 added to allow 3rd condition, also allowing AND/OR   */
    match = 0;          /* presume no match */
    if ((sargs.a3cc[0] != '|') && (sargs.a5cc[0] != '|'))
      { if ((p1d >= 0) && (p2d >= 0) && (p3d >= 0))
            match = 1;
      }
    else if ((sargs.a3cc[0] != '|') && (sargs.a5cc[0] == '|'))
      { if ((p1d >= 0) && ((p2d >= 0) || (p3d >= 0)))
            match = 1;
      }
    else if ((sargs.a3cc[0] == '|') && (sargs.a5cc[0] != '|'))
      { if ((p1d >= 0) || ((p2d >= 0) && (p3d >= 0)))
            match = 1;
      }
    else if ((sargs.a3cc[0] == '|') && (sargs.a5cc[0] == '|'))
      { if ((p1d >= 0) || (p2d >= 0) || (p3d >= 0))
            match = 1;
      }
    if (!match)
      { return(0);
      }
  }
    
/*eject*/
/* disable search patterns not within current segment                */
/* - by setting dsplcmnts high & length zero                         */
if ((p1d < rii1) || (p1d >= rii))
  { p1d = 262144;
    p1l = 0;
  }
if ((p2d < rii1) || (p2d >= rii))
  { p2d = 262144;
    p2l = 0;
  }
if ((p3d < rii1) || (p3d >= rii))
  { p3d = 262144;
    p3l = 0;
  }

/* if patterns spans segments truncate length to within current segment*/
lincde = rii1 + lincl;        /* calc dsplcmnt to end current segment */
p1de = p1d + p1l;             /* calc dsplcmnt to end pattern1        */
p2de = p2d + p2l;             /* calc dsplcmnt to end pattern2        */
p3de = p3d + p3l;             /* calc dsplcmnt to end pattern3        */

if ((p1de < 262144) && (p1de > lincde))
  { p1l = (lincde -p1d);
  }
if ((p2de < 262144) && (p2de > lincde))
  { p2l = (lincde -p2d);
  }
if ((p3de < 262144) && (p3de > lincde))
  { p3l = (lincde -p3d);
  }

/* recalc end dsps in case of length change for patterns spanning segments*/
p1de = p1d + p1l;             /* recalc dsplcmnt to end pattern1          */
p2de = p2d + p2l;             /* recalc dsplcmnt to end pattern2          */
p3de = p3d + p3l;             /* recalc dsplcmnt to end pattern3          */

/* bypass if no search patterns within current line segment   */
if ((p1l == 0) && (p2l == 0) && (p3l == 0))
  { return(0);
  }

/*eject*/
/* load table of start/end dsplcmnts to be bolded               */
/* - relative to beginning of current segment                   */
/* - so we can sort by start dsplcmnt                           */
bt[0] = p1d-rii1; bt[1] = p1de-rii1;
bt[2] = p2d-rii1; bt[3] = p2de-rii1;
bt[4] = p3d-rii1; bt[5] = p3de-rii1;
bt[6] = 262144; bt[7] = 262144; /* mark end table with high entries   */
sortint2(&bt[0],3);             /* sort table start/end pairs by start*/

/* disable any overlapping entries                                    */
/* - if next start dsplcmnt < current end dsplcmnt, disable next pair */
for (tt=0; tt < 6; tt+=2)
  { if (bt[tt+2] < bt[tt+1])
      { bt[tt+2] = 262144;   /* disable start via hi index never reached */
        bt[tt+3] = -1;       /* disable end via -1 to protect folwng pair*/
      }
  }

/*Nov02/14 - disable 0,0 entries */
if ((bt[0] == 0) && (bt[1] == 0))
  { bt[0] = 262144; bt[1] = 262144;
  }
if ((bt[2] == 0) && (bt[3] == 0))
  { bt[2] = 262144; bt[3] = 262144;
  }
if ((bt[4] == 0) && (bt[5] == 0))
  { bt[4] = 262144; bt[5] = 262144;
  }
sortint2(&bt[0],3);          /* re-sort table to move any disableds high */

/* init I/O & table indices to copy data inserting bold stop/start patterns*/
ii = 0;                      /* index for input data segment           */
jj = 0;                      /* index for output area                  */
tt = 0;                      /* index for table of start/end dsplcmnts */

/* use while loop to copy byte by byte                                 */
/* - testing current input dsplcmnt for match in table                 */
/*   of start/stop dsplcmnts to be bolded                              */
/* when start/stop dsplcmnts match,  insert start/stop bold patterns   */
while (ii < lincl)
  { if (ii == bt[tt])               /* current dsp = start bold dsp ?  */
      { memcpy(linp+jj,smso,smsol); /* insert start bold escape seq    */
        jj += smsol;                /* up output index by lth startbold*/
      }
    linp[jj++] = linc[ii++];        /* copy current byte & up indices  */
    if (ii == bt[tt+1])             /* current dsp = stop bold dsp ?   */
      { memcpy(linp+jj,rmso,rmsol); /* insert stop  bold escape seq    */
        jj += rmsol;                /* up output index by lth stopbold */
        tt += 2;                    /* up bold table to next pair      */
      }
  }
linp[jj] = '\0';                    /* null terminate            */
return(1);
}

/*eject*/
/*uvhd------------------------------ sortint2 ---------------------------*/
/* sortint2 - buble sort for tables of integer pairs                     */
/*          - sorts on 1st integer & moves pairs as indicated            */

short sortint2(int *tbl, int ents)
{
int ii;              /* index to table of integer pairs    */
int s1,s2;           /* save areas for current pair switch */
int flips;           /* flip counter for current pass      */

while(1)
  { flips = 0;                      /* reset flip ctr for current pass */

    for (ii=0; ii < ents; ii+=2)
      { if (tbl[ii+2] < tbl[ii])
          { s1 = tbl[ii];            /* save current pair              */
            s2 = tbl[ii+1];
            tbl[ii] = tbl[ii+2];     /* move next pair to current pair */
            tbl[ii+1] = tbl[ii+3];
            tbl[ii+2] = s1;          /* move saved pair to next pair   */
            tbl[ii+3] = s2;
            flips++;                 /* count flips this pass          */
          }
      }
    /* end current pass - if no flips, we are done                      */
    if (flips == 0)
      { break;
      }
  }
return(1);
}

/*eject*/
/*uvhd--------------------------- parse --------------------------------*/
/* parse the command entered (in reply to prompt)                       */
/* - return 1 OK, 0 if errors discovered                                */
/*                                                                      */
/* type1 - 1 char command + optional numeric value                      */
/* r100                     - goto & display record #100                */
/*                                                                      */
/* type2 - cmd + op1,op2                                                */
/* s 0(20),='ABC'             - search for ABC within 1st 20 bytes      */
/* s 0(3),!'ABC'              - search for NOT ABC in cols 1-3          */
/*                              <!> search lth must match if lth > 1    */
/* p 0(3),>'800'              - print records with > 800 in 1st 3 bytes */
/* u 0(3),='ABC'              - update cols 1-3 of current rec w ABC    */
/*                                                                      */
/* s ='ABC'                   - search for ABC anywhere in entire record*/
/*                            - op1 dflts to 0(rcsz) if omitted         */
/*                                                                      */
/* provide 6 operands for commands (search, write, etc)                 */
/* - cmd + op1,op2,op3,op4,op5,op6 (allow 3 conditions in 3 diff fields)*/
/*                                                                      */
/* s 0(40),='ABC',40(40),='XYZ' - search for ABC in 1-40 & XYZ in 41-80 */
/*                                                                      */
/* s 0(1),='1',40(1),='A'   - search for 1 in byte 0 & A in byte 40     */
/* s 0(1),'1',&40(1),'A'    - same ('=' and '&' are the defaults)       */
/* s 0(1),'1',|40(1),'A'    - search for 1 in byte 0 OR A in byte 40    */
/*                                                                      */
/* s 0(80),<x'20',0(80),>x'7E' - search for non printable chars in 1-80 */
/*                                                                      */
/* s 0(1),'1',|,'2'         - op3 omitted defaults to op1 search area   */
/*                          - must specify OR (|) for = conditions      */
/* s 0(1),>'1',,<'9'        - search for 2-8 in byte 0 (range search)   */
/*                                                                      */
/*Dec06/10 - add move field/constant command */
/* m  35(30),90(12)         - move tel# & clear extra op1 lth           */
/* m  50(30),'Dec 06/2010'  - move constant & clear extra lth           */

int parse(void)
{
int sdi;                   /* dsplcmnt for sdscopy function           */
int ss;                    /* status of various cmnds                 */
char *pp,*qq;              /* ptrs returned by subfuns (atol1,etc)    */
int ee;                    /* end op1 (dsp + lth)                     */
int ii,jj;                 /* misc index                              */
char a3ccs[4];             /* save | or & from op4 for op3 and/or     */
char a5ccs[4];             /* save | or & from op6 for op5 and/or     */
char arg2ctx = '\0';       /* op2 constant type explicit c/e/x        */
char arg4ctx = '\0';       /* op4 constant type explicit c/e/x        */
char arg6ctx = '\0';       /* op6 constant type explicit c/e/x        */

/*Dec19/10 - add fields for DataType & DataCode carg.a1dt carg.a1dc   */
char c1,c2;
char dt1;     /* DataType (c/z/p/f) default c, store from 120(5p) etc */
char dc1;     /* DataCode (a/e) default a, store from 54(8ze) etc     */
char dt2;     /*Dec26/10 - add for op2 on move numeric                */ 
char dc2;

/*eject*/
/* clear argument storage from prior commands */
memset(cms,'\0',80);   memset(args,'\0',80);
memset(arg1a,'\0',80); memset(arg1b,'\0',80); memset(arg1c,'\0',80);
memset(arg2a,'\0',80); memset(arg2b,'\0',80); memset(arg2c,'\0',80);
memset(arg3a,'\0',80); memset(arg3b,'\0',80); memset(arg3c,'\0',80);
memset(arg4a,'\0',80); memset(arg4b,'\0',80); memset(arg4c,'\0',80);
memset(arg5a,'\0',80); memset(arg5b,'\0',80); memset(arg5c,'\0',80);
memset(arg6a,'\0',80); memset(arg6b,'\0',80); memset(arg6c,'\0',80);
memset(a3ccs,'\0',4);  memset(a5ccs,'\0',4);
arg1d = 0; arg1l = 0; 
arg2d = 0; arg2l = 0;
arg3d = 0; arg3l = 0; 
           arg4l = 0;
arg5d = 0; arg5l = 0; 
           arg6l = 0;
op1dflt = 0;
arg1x=0; arg3x=0; arg5x=0; /* clear arg3 & arg5 omitted switches */

/*Dec19/10 - add fields for DataType & DataCode carg.a1dt carg.a1dc*/
dt1 = 'c';               /* default DataType 'c' Character */
dc1 = 'a';               /* default DataCode 'a' Ascii     */
dt2 = 'c';               /*Dec26/10 ad for op2 move numeric*/ 
dc2 = 'a';           

/*Feb26/11 - if cmd line option 'a', chg default to EBCDIC */
if (opsc['a'-'a'])
  { dc1 = 'e';           
    dc2 = 'e';           
  }
/*Jan21/09 - if no cmd entered - return now */
if (opcmd[0] <= ' ')
  { return(1);
  }
/* if opcmd 1st byte numeric - default to the 'r' (rec#) command       */
if (isdigit(opcmd[0]))
  { strcpy(opcmd2,opcmd);               /* copy cmd to alt w/s         */
    /*Nov22/10 - OR default to 'f' find for variable length files      */
    if (ftype == 'f')
      { opcmd[0] = 'r';                 /* insert 'r' command          */
      }
    else
      { opcmd[0] = 'f';                 /* insert 'f' command          */
      }
    strcpy(opcmd+1,opcmd2);             /* append original cmd string  */
  }

/* separate command & arguments assuming: cmd op1,op2,op3,op4          */
/* s 0(40),='ABC',40(40),='XYZ'    - search ABC in 1-40 & XYZ in 41-80 */
sdscopy(cms,opcmd,80,&sdi,"~~ ~",0x1032);
sdscopy(args,opcmd,80,&sdi,"~~~~",0x2032);
strcpy(cops,cms);                          /* cmd+options               */

/*Nov09/07 - return now if cmd 'q' (quit) */
if (cops[0] == 'q')
  { return(9);        /* return > 0 to avoid reprompt */
  }

/*eject*/
/* errchk - space after cmnd & prior to arguments                       */
/*        - should be no ( ) , ' " in cmnd string                       */
ss = searchcm(cms,"(),\'\"",20,5,0x03);
if (ss >= 0)
  { errmsg("command format - no space between cmd & args","","",0,0x10);
    return(0);
  }

/* process cmd options, 1st cnvrt non lower cmds - to prevent sortops err*/
if (cops[0] == '+') cops[0] = 'o';
if (cops[0] == '-') cops[0] = 'o';
if (cops[0] == '?') cops[0] = 'o';
if (cops[0] == ':') cops[0] = 'o';
if (cops[0] == 'X') cops[0] = 'o';
if (cops[0] == 'R') cops[0] = 'o';

/* sort/store options, but not if 2nd byte '=' (transfer last search cmd)*/
if (cops[1] != '=')
  { ss = sortops(cops,copsc,copsi,0x10);
    if (ss < 0)
       return(0);
  }

/* store any numeric value appended to the command                    */
/* - using option for cmd letter to avoid any option numeric values   */
/* cmn = copsi[cops[0] - 'a']; Jun05/10 does not allow > 2 gig        */

/*Jun05/10 - allow numeric values > 2 gig on cmds such as b2200000000 */
/* extract numeric digits until non-numeric encountered               */
memset(copsn,'\0',24);
ii=1; jj=0;
/*Dec06/10 - allow count on repeat cmd (uu99, mm99, etc) */
if (cops[1] == cops[0])
  { ii = 2;
  }
while (ii < 24)
  { if (!(isdigit(cops[ii])))
      { break;
      }
    copsn[jj++] = cops[ii++];
  }
/* cmn = strtol(copsn,(char**)0,10); May19/11*/
/* cmn = atol(copsn); Oct04/11 change back for 32bit gcc HP Itanium */
cmn = strtoll(copsn,(char**)0,10);
/*Dec06/10 - convert 99 to 999,999,999 */
/*Jan10/11 - but not for find command */
if ((cmn == 99) && (opcmd[0] != 'f'))
  { cmn = 999999999;
  }

/* separate args based on commas ie:  0(20),'ABC',40(20),'XYZ'        */
sdscopy(arg1a,args,80,&sdi,"~~,~",0x1012);
sdscopy(arg2a,args,80,&sdi,"~~,~",0x2012);
sdscopy(arg3a,args,80,&sdi,"~~,~",0x2012);
sdscopy(arg4a,args,80,&sdi,"~~,~",0x2012);
sdscopy(arg5a,args,80,&sdi,"~~,~",0x2012);
sdscopy(arg6a,args,80,&sdi,"~~~~",0x2002);

/*eject*/
/* allow op1 dsp(lth) to be omitted - default to 0(rcsz) entire record */
/* - if quote found in 1st 3 bytes of op1, assume op1 omitted          */
if (memchr(arg1a,'\'',3))
  { arg1x = 1;           /*Mar24/14 set switch arg1 omitted, const->op2*/
    strcpy(arg6a,arg5a);               /* shift op5 to op6, etc        */
    strcpy(arg5a,arg4a);
    strcpy(arg4a,arg3a);
    strcpy(arg3a,arg2a);
    strcpy(arg2a,arg1a);
    sprintf(arg1a,"0(%d)",rszo);       /* default op1 to 0(rcsz)       */
    /*Jun04/10 - chg above from rsz1 to rszo for varlth files          */
    op1dflt = 1;                       /* store indicator for scan/rep */
  }
/* allow op3 dsp(lth) to be omitted - default to op1 dsp(lth)          */
/*Feb23/14 - if op3 omitted, set switch search entire rec if lth mismatch*/
if (!(arg3a[0]))
  { arg3x = 1;
  }
/* - if quote found in 1st 3 bytes of op3, assume op3 omitted          */
if (memchr(arg3a,'\'',3))
  { strcpy(arg6a,arg5a);               /* shift op5 to op6, etc        */
    strcpy(arg5a,arg4a);
    strcpy(arg4a,arg3a);
    strcpy(arg3a,arg1a);               /* default op3 to op1 dsp(lth)  */
    arg3x = 1;              /* dflt entire record if arg4 lth mismatch */
  }
/* allow op5 dsp(lth) to be omitted - default to op3 dsp(lth)          */
/*Feb23/14 - if op5 omitted, set switch search entire rec if lth mismatch*/
if (!(arg5a[0]))
  { arg5x = 1;
  }
/* - if quote found in 1st 3 bytes of op5, assume op5 omitted          */
if (memchr(arg5a,'\'',3))
  { strcpy(arg6a,arg5a);               /* shift op5 to op6             */
    strcpy(arg5a,arg3a);               /* default op5 to op3 dsp(lth)  */
    arg5x = 1;              /* dflt entire record if arg6 lth mismatch */
  }

/* error check - arg1 (if coded), must include '('                   */
if ((arg1a[0]) && (strchr(arg1a,'(') == 0))
  { errmsg("command format - no '(' in arg1","","",0,0x10);
    return(0);
  }

/* default op3 same as op1 - if op3 omitted or only '|'              */
/* s 0(40),='ABC',|0(40),='XYZ'  - search ABC in 1-40 OR XYZ in 1-40 */
/* s 0(40),='ABC',|,='XYZ'       - same by default                   */
if (arg4a[0])
  { if (arg3a[0] < '0')
        strcpy(arg3a,arg1a);
    else if ((arg3a[0] == '|') && (arg3a[1] < '0'))
        strcat(arg3a,arg1a);
  }

/*eject*/
/* default op5 same as op3 - if op5 omitted or only '|'              */
/* s 10(3),='AAA',|20(3),>'BBB',|30(3),<'CCC'                        */
/* s 10(3),='AAA',|,>'BBB',|,<'CCC'                                  */
if (arg6a[0])
  { if (arg5a[0] < '0')
        strcpy(arg5a,arg3a);
    else if ((arg5a[0] == '|') && (arg5a[1] < '0'))
        strcat(arg5a,arg3a);
  }

/* process arg1 - into displacement & length */
strcpy(arg1b,arg1a);
pp = atol1(&arg1d,arg1b,'(',6,0x05);
qq = atol1(&arg1l,pp,')',6,0x01);

/*Dec19/10 - retrieve DataType & DataCode for getop, acum1, etc */
/* a 120(5p), a 54(9ze), a 0(4bs), etc                          */
/* retrieve 1 or 2 alpha codes preceding ')' closing length     */
c1 = *(qq-1);
c2 = *(qq-2);
if ((isalpha(c1)) && (isalpha(c2)))
  { dt1 = c2;
    dc1 = c1;
  }
else if (isalpha(c1))
  { dt1 = c1;
  }

/*eject*/
/*Dec06/10 - allow op2 dsp(lth) for new move instrn */
if (arg2a[0])                   /* if op2 specified */
  { /* assume constant if single quote found, else assume dsp(lth) */
    if (strchr(arg2a,'\'')) 
      { /* standardize op2 constant in arg2b, default omitted condtn/type*/
        /*Dec07/10 - AEC constant type default ASCII=c or optn a EBCDIC=e*/ 
        memcpy(arg2b,AEC,2);          /* setup dflts for condition & type*/
        ii = 0;
        if ((arg2a[ii] == '=') || (arg2a[ii] == '!') ||
            (arg2a[ii] == '<') || (arg2a[ii] == '>'))
          { arg2b[0] = arg2a[ii++];
          }
        if ((arg2a[ii] == 'x') || (arg2a[ii] == 'X') ||
            (arg2a[ii] == 'c') || (arg2a[ii] == 'C') ||
            (arg2a[ii] == 'e') || (arg2a[ii] == 'E'))
          { arg2ctx = arg2a[ii];        /* save for explicit test below */
            arg2b[1] = arg2a[ii++];
          }
        strcpy(&arg2b[2],&arg2a[ii]);   /* copy op2 constant '...data...' */
    
        /* error check - arg2b must now include quote in 3rd byte         */
        if (arg2b[2] != '\'')
          { errmsg("arg2 constant condition/type/open-quote invalid"
                   ,"","",0,0x10);
            return(0);
          }
        /* process arg2 - strip quotes & cnvrt any hex rep to true hex */
        if ((arg2b[1] == 'x') || (arg2b[1] == 'X'))
          { memset(arg2c,'\0',40);
            arg2l = hex2data(arg2c,&arg2b[3],'\'',80,0x03);
            dt2 = 'x';     /*Mar24/14 - set data type = hex */
          }
        else
          { arg2l = stncopy(arg2c,&arg2b[3],'\'',80,0x03);
            /* test const type 'e' convert ASCII to EBCDIC (cmd line optn a)*/
            /* Dec2002 - assume EBCDIC e'---' if optn 'a' spcfd on cmd line */
            /* if ((arg2b[1] == 'e') || (arg2b[1] == 'E') || (opsc['a'-'a'])) */
            /*Dec07/10 - remove optn a test here - constant type set above */
            if ((arg2b[1] == 'e') || (arg2b[1] == 'E'))
              { toebcdic2((Uchar*)arg2c,arg2l,0x00); /* cnvrt ASCII to EBCDIC*/
              }
          }
      }
/*eject*/
    else
      { /*Dec06/10 - assume dsp(lth) since no 'quote found in op2  */
        /*         - allow op2 (lth) optional                      */
        /*Dec26/10 - allow 18 bytes for op2 value (op2 lth omitted)*/
        pp = atol3(&arg2d,arg2a,"(, ",18,0x01);
        if (*pp == '(')
          { qq = atol3(&arg2l,pp+1,"), ",6,0x01);
          }
        /* if op2 lth omitted default to op1 lth */
        /*Dec26/10 - but not if move Numeric     */
        if (cops[0] == 'n')
          { ;
          }
        else if (arg2l == 0)
          { arg2l = arg1l;
          }
        /*Dec26/10 - retrieve op2 Type&Code for getop, acum1, etc  */
        /* retrieve 1 or 2 alpha codes preceding ')' closing length*/
        c1 = *(qq-1);
        c2 = *(qq-2);
        if ((isalpha(c1)) && (isalpha(c2)))
          { dt2 = c2;
            dc2 = c1;
          }
        else if (isalpha(c1))
          { dt2 = c1;
          }
      }
  }

/*eject*/
/* standardize op4 constant in arg4b by defaulting omitted condtn/type*/
if (arg4a[0])                   /* if op4 constant specified         */
  { /*Dec07/10 - AEC constant type default ASCII=c or optn a EBCDIC=e*/ 
    memcpy(arg4b,AEC,2);       /* setup dflts for condition & type   */
    ii = 0;
    if ((arg4a[ii] == '|') || (arg4a[ii] == '&'))
      { a3ccs[0] = arg4a[ii++]; /* save |/& AND/OR indicator */
      }
    if ((arg4a[ii] == '=') || (arg4a[ii] == '!') ||
        (arg4a[ii] == '<') || (arg4a[ii] == '>'))
      { arg4b[0] = arg4a[ii++];
      }
    if ((arg4a[ii] == 'x') || (arg4a[ii] == 'X') ||
        (arg4a[ii] == 'c') || (arg4a[ii] == 'C') ||
        (arg4a[ii] == 'e') || (arg4a[ii] == 'E'))
      { arg4ctx = arg4a[ii];        /* save for explicit test below */
        arg4b[1] = arg4a[ii++];
      }
    strcpy(&arg4b[2],&arg4a[ii]);   /* copy op4 constant '...data...' */

    /* error check - arg4b must now include quote in 3rd byte         */
    if (arg4b[2] != '\'')
      { errmsg("arg4 constant condition/type/open-quote invalid","","",0,0x10);
        return(0);
      }
  }

/* process arg3 - into displacement & length                          */
strcpy(arg3b,arg3a);
pp = atol1(&arg3d,arg3b,'(',6,0x05);
     atol1(&arg3l,pp,')',6,0x01);

/* process arg4 - strip quotes & cnvrt any hex rep to true hex        */
if ((arg4b[1] == 'x') || (arg4b[1] == 'X'))
  { arg4l = hex2data(arg4c,&arg4b[3],'\'',80,0x03);
  }
else
  { arg4l = stncopy(arg4c,&arg4b[3],'\'',80,0x03);
    /* test constant type 'e' convert ASCII to EBCDIC (for cmd line optn a)*/
    /* Dec2002 - assume EBCDIC e'---' if option 'a' spcfd on cmd line      */
    /*Dec07/10 - remove optn a test here - constant type set above */
    if ((arg4b[1] == 'e') || (arg4b[1] == 'E'))
      { toebcdic2((Uchar*)arg4c,arg4l,0x00);      /* cnvrt ASCII to EBCDIC */
      }
  }

/*Feb23/14 - if op3 dsp(lth) omitted it defaults to op1 dsp(lth)           */ 
/*         - but if op4 constant lth not= op1 lth, deflt op3 entire record */
if ((arg3x) && (arg4l != arg1l))
  { arg3d = 0;
    arg3l = rszo;
  }

/*eject*/
/* standardize op6 constant in arg6b by defaulting omitted condtn/type*/
if (arg6a[0])                  /* if op6 constant specified          */
  { /*Dec07/10 - AEC constant type default ASCII=c or optn a EBCDIC=e*/ 
    memcpy(arg6b,AEC,2);       /* setup dflts for condition & type   */
    ii = 0;
    if ((arg6a[ii] == '|') || (arg6a[ii] == '&'))
      { a5ccs[0] = arg6a[ii++]; /* save |/& AND/OR indicator */
      }
    if ((arg6a[ii] == '=') || (arg6a[ii] == '!') ||
        (arg6a[ii] == '<') || (arg6a[ii] == '>'))
      { arg6b[0] = arg6a[ii++];
      }
    if ((arg6a[ii] == 'x') || (arg6a[ii] == 'X') ||
        (arg6a[ii] == 'c') || (arg6a[ii] == 'C') ||
        (arg6a[ii] == 'e') || (arg6a[ii] == 'E'))
      { arg6ctx = arg6a[ii];        /* save for explicit test below */
        arg6b[1] = arg6a[ii++];
      }
    strcpy(&arg6b[2],&arg6a[ii]);   /* copy op6 constant '...data...' */

    /* error check - arg6b must now include quote in 3rd byte         */
    if (arg6b[2] != '\'')
      { errmsg("arg6 constant condition/type/open-quote invalid","","",0,0x10);
        return(0);
      }
  }

/* process arg5 - into displacement & length                          */
strcpy(arg5b,arg5a);
pp = atol1(&arg5d,arg5b,'(',6,0x05);
     atol1(&arg5l,pp,')',6,0x01);

/* process arg6 - strip quotes & cnvrt any hex rep to true hex        */
if ((arg6b[1] == 'x') || (arg6b[1] == 'X'))
  { arg6l = hex2data(arg6c,&arg6b[3],'\'',80,0x03);
  }
else
  { arg6l = stncopy(arg6c,&arg6b[3],'\'',80,0x03);
    /* test constant type 'e' convert ASCII to EBCDIC (for cmd line optn a)*/
    /* Dec2002 - assume EBCDIC e'---' if option 'a' spcfd on cmd line      */
    /*Dec07/10 - remove optn a test here - constant type set above */
    if ((arg6b[1] == 'e') || (arg6b[1] == 'E'))
      { toebcdic2((Uchar*)arg6c,arg6l,0x00);      /* cnvrt ASCII to EBCDIC */
      }
  }

/*Feb23/14 - if op5 dsp(lth) omitted it defaults to op3 dsp(lth)           */ 
/*         - but if op6 constant lth not= op1 lth, deflt op5 entire record */
if ((arg5x) && (arg6l != arg1l))
  { arg5d = 0;
    arg5l = rszo;
  }

/*eject*/
/*Dec2002 - if option 'g1' on cmd line for 1 relative (vs 0 relative)  */
/*        - subtract 1 from dsplcmnts (force 0 if would be negative)   */
/*Mar25/14 - copied parse() from uvhd to uvhdcob */
/* - same except, change option g1 (col1 relative) to c1 for uvhdcob */
if (opsi['g'-'a'] & 0x01)
  { arg1d--;
    arg3d--;
    arg5d--;
    if (arg1d < 0) arg1d = 0;
    if (arg3d < 0) arg3d = 0;
    if (arg5d < 0) arg5d = 0;
  }

/* store results in a structure, for easy move depending on s,p,w,u   */
cargs = carg0;                        /* clear any prior data         */
strcpy(cargs.cmd,opcmd);
strcpy(cargs.cmc,cms);
cargs.cmv = cmn;
memcpy(cargs.a2c,arg2c,arg2l);
memcpy(cargs.a4c,arg4c,arg4l);
memcpy(cargs.a6c,arg6c,arg6l);
memcpy(cargs.a1cc,arg1b,3);
memcpy(cargs.a2cc,arg2b,3);
memcpy(cargs.a3cc,arg3b,3);
memcpy(cargs.a4cc,arg4b,3);
memcpy(cargs.a5cc,arg5b,3);
memcpy(cargs.a6cc,arg6b,3);

/* extract command options (dropping command & count) */
/* ex: if cms="t999999c15" then cmo="c15"             */
for (ii=1,jj=0; cms[ii]; ii++)
  { if (cms[ii] == cms[0])
        continue;
    if (!(isdigit(cms[ii])))
        break;
  }
while(cms[ii])
  { cargs.cmo[jj++] = cms[ii++];
  }

/* store any &/| AND/OR ccs saved from op4 for op3 or from op6 for op5 */
if (a3ccs[0])
    memcpy(cargs.a3cc,a3ccs,3);
if (a5ccs[0])
    memcpy(cargs.a5cc,a5ccs,3);
cargs.a1d = arg1d;
cargs.a1l = arg1l;
cargs.a2d = arg2d;   /*Dec06/10 for move instrn */
cargs.a2l = arg2l;
cargs.a3d = arg3d;
cargs.a3l = arg3l;
cargs.a4l = arg4l;
cargs.a5d = arg5d;
cargs.a5l = arg5l;
cargs.a6l = arg6l;

/*eject*/
/*Dec19/10 - store DataType & DataCode for getop, acum1, etc */
cargs.a1dt = dt1;
cargs.a1dc = dc1;
cargs.a2dt = dt2;
cargs.a2dc = dc2;

memcpy((char*)cargs.opi,(char*)copsi,104);
memcpy(cargs.opc,copsc,26);
cargs.op1dflt = op1dflt;
/*May16/08 - store indicator op1 dfltd to 0(rsz1)          */
/* - used by scan/rep to set op1lth = rsz1 for each record */
/*Jun04/10 - chg above from rsz1 to rszo for varlth files  */

/*Jun02/10 - chk op1 dsp+lth not > recsize                 */
/* - only if lth spcfd & recsize > 0                       */
/* - could be 0 at EOF when enter 0/1 return to BOF        */
/*Jun04/10 - chg recsize from rsz1 to rszo for varlth files*/
/*Jun14/11 - fix ERR dsp+lth > rcsz for varlth recs when lth dflt maxsize  */
/*         - just check dsp not> rcsz (ignore lth)                         */
ee = cargs.a1d + cargs.a1l;
if ((cargs.a1l) && (rcsmax) && (cargs.a1d > (rcsmax + vrhs)))
  { errmsg("op1 start-byte > rcsz","","",0,0x10);
    return(0);
  }

/* errcheck >!< compares,                                              */
/* if search pattern multibyte, search area lth must = pattern lth     */
if ((cargs.a2cc[0] == '>') || (cargs.a2cc[0] == '<') || (cargs.a2cc[0] == '!'))
 { if ((cargs.a2l > 1) && (cargs.a1l != cargs.a2l))
  { errmsg("compare <!> multi-byte pattern, search area lth must = pattern lth"
            ,"","",0,0x10);
    return(0);
  }
 }

return(1);
}

/*eject*/
/*uvhd------------------------ search1 --------------------------------*/
/* search for a data pattern - forward (backward removed June2010)     */
/*May25/10 - backward search removed (upnext forward in getrec)        */
/*                                                                     */
/* s 0(20),'ABC',20(20),'XYZ'   - search for ABC within 1st 20 bytes   */
/*                                & XYZ in bytes 20-39                 */
/*                                                                     */
/* ss                           - repeat previous search               */
/*                                                                     */
/* - set fileptr to 1st byte of record containing pattern & return 1   */
/* - if nofind set fileptr to last rec in file & return 0              */

int search1(void)
{
int rr;              /* max records search ctr             */
int mm,nn,oo;        /* field1,2,3 search find/nofind 0+/- */
int mmd,nnd,ood;     /* srch1,2,3 dsplcmnt in rec or file  */
int match;           /* match results of 3 conditions 0/1  */
int nth;             /* search for nth occurrence (optn n) */
int ss;              /* search occurrence# counter         */

rr = 0;              /* reset max search ctr               */
nth = 0;             /* reset nth occurrence match ctr     */

upnext = 1;          /* ensure getrec advance to next record */

/* test for new search or repeat previous search                     */
/* note re pscd - op1 dsp prior match (for search continue)          */
/* cmd ss  - repeat search (from next record)                        */
/* cmd sss - repeat search (same record, incrmnt column)             */
if (cms[1] == 's')           /* repeat search ?           */
  { if (cms[2] == 's')       /* repeat samerec incrmnt col*/
      { sargs.pscd += 1;     /* yes - incrmnt dsp start   */
        upnext = 0;          /* inhibit getrec advance    */
      }
    else
      { /* rsz1 = getrec(0);  Nov20/10 - remove */
        /*Nov20/10 - fix bug ss missing match in 1st rec after a match */
        /*         - upnext=1 above & getrec at begin loop should work */
        sargs.pscd = 0;      /* ensure dsp extra clear    */
      }
  }
else
  { /*Sep16/17 - clear vrtyp2 for new search, but not ss */
    vrtyp2 = '\0'; 
    /* store new search args - but 1st verify (cargs here, sargs below)*/
    /* if ((cargs.a1l <= 0) || (cargs.a2l <= 0)) */
    /*Feb23/14 - test op1 lth only, allow op2 lth 0 for highlight */
    if (cargs.a1l <= 0)
      { errmsg("search args invalid",cargs.cmd,"",0,0x10);
        return(0);
      }
    sargs = cargs;       /* store new search args                  */
    upnext = 0;          /* inhib advance to search current record */
  }

/*eject*/
/*Feb23/14 - highlight search op1 field without search match */
/*         - if op1 dsp(lth) spcfd & op2 constant omitted    */
/*           store highlight dsplcmnt & length               */
hil1d = 0; hil1l = 0;
hil2d = 0; hil2l = 0;
hil3d = 0; hil3l = 0;
/* if ((sargs.a1d > 1) && (sargs.a2l == 0)) */
/*Nov02/14 - change from above to below 'a1d > 1' wrong ? */
if ((sargs.a1l > 0) && (sargs.a2l == 0))
  { hil1d = sargs.a1d;
    hil1l = sargs.a1l;
    /* op3 highlight allowed if dsplcmnt > 0 since op3 lth defaults reclth */
    if ((sargs.a3l > 0) && (sargs.a3d > 0))
      { hil2d = sargs.a3d;
        hil2l = sargs.a3l;
      }
    /* op5 highlight allowed if dsplcmnt > 0 since op5 lth defaults reclth */
    if ((sargs.a5l > 0) && (sargs.a5d > 0))
      { hil3d = sargs.a5d;
        hil3l = sargs.a5l;
      }
    rsz1 = getrec(0);           /* get next record */
    return(2);
  }
else
  { /* verify search args - op1 lth & op2 constant lth > 0 ?             */
    /*Nov02/14 - test op1 lth only, allow op2 lth 0 for highlight */
    /* if ((sargs.a1l <= 0) || (sargs.a2l <= 0)) */
    if (sargs.a1l <= 0)
      { errmsg("search args invalid",sargs.cmd,"",0,0x10);
        return(0);
      }
  }

/*Mar24/14 - allow search binary value */
/* --> s 65(4b),123456789 <-- example  */
/* op2 already converted to binary in sargs.a2d */
/* - move to constant search arg sargs.a2c      */
/* - need to switch binary ends if Little End Machine */
if (sargs.a1dt == 'b')
  { memcpy(sargs.a2c, (char*)&(sargs.a2d), sargs.a1l);
    if (((lem) && (sargs.a1dc == 's')) || ((bem) && (sargs.a1dc != 's')))
      { if (sargs.a1l == 2)
          { bswap2((Uchar*)sargs.a2c);
          }
        else if (sargs.a1l == 4)
          { bswap4((Uchar*)sargs.a2c);
          }
        else if (sargs.a1l == 8)
          { bswap8((Uchar*)sargs.a2c);
          }
      }
  }

/*eject*/
/* search for pattern (char or hex) until found or EOF                */
pmsgctr = 0;             /* init search progress msg counter */
/*Apr23/14 - test option o# search occurrence# of pattern within record */
if (sargs.opi['o'-'a'])
  { upnext =0;           /* inhibit advance to next record */
  }
while (1)
  { rsz1 = getrec(0);                 /* get next record               */
    if (rsz1 <= 0)                    /* EOF ?                         */
      { fileptr = filesiz0;
        printf("EOF w nomatch to: %s - reset BOF (r0) & repeat(ss) ?\n"
               ,sargs.cmd);
        return(0);
      }

    /*Aug07/08 - updt search arg1 lth with current recsz if op1 lth default*/
    if (op1dflt)
      { sargs.a1l = rsz1;
      }

    /*May28/11 - test search progress message interval record count  */
    pmsgctr++;               /* increment search msg interval counter*/
    if (pmsgctr >= pmsgcnt)
      { printf("%s at rec# %d of %d\n",sargs.cmd,recnum1,rcount);
        pmsgctr = 0;        /* reset ctr for next msg */
      }

    /* search for patterns - char strings or hex patterns              */
    /*May2003 - 3 possible conditions = < > & AND/OR possible relations*/
    /*note - might have used 'search234' function to do this           */
    /*     - BUT we need to capture the dsplcmnts for reporting        */
    /*       (from returns of separate search2, search3,& search4)     */

    mm = search2(recb,&sargs);       /* 9701 search = > < !            */

    /* search for op4 pattern if specified                             */
    if (sargs.a4l)
      { nn = search3(recb,&sargs);  /* search for match on op4 constant*/
      }
    else
      { nn = 32767;                 /* force match if op4 omitted      */
      }
    /*Feb2003 - 32767 used so will not attempt BOLD when no op4 pattern*/

    /* search for op6 pattern if specified                             */
    if (sargs.a6l)
      { oo = search4(recb,&sargs);  /* search for match on op4 constant*/
      }
    else
      { oo = 32767;                 /* force match if op4 omitted      */
      }

/*eject*/
    /* test results of 3 searches depending on conditions vs AND/ORs possible*/
    /*May2003 - op4/op5 added to allow 3rd condition, also allowing AND/OR   */

    match = 0;  /* presume no-match */
    if ((sargs.a3cc[0] != '|') && (sargs.a5cc[0] != '|'))
      { if ((mm >= 0) && (nn >= 0) && (oo >= 0))
            match = 1;
      }
    else if ((sargs.a3cc[0] != '|') && (sargs.a5cc[0] == '|'))
      { if ((mm >= 0) && ((nn >= 0) || (oo >= 0)))
            match = 1;
      }
    else if ((sargs.a3cc[0] == '|') && (sargs.a5cc[0] != '|'))
      { if ((mm >= 0) || ((nn >= 0) && (oo >= 0)))
            match = 1;
      }
    else if ((sargs.a3cc[0] == '|') && (sargs.a5cc[0] == '|'))
      { if ((mm >= 0) || (nn >= 0) || (oo >= 0))
            match = 1;
      }

    /*Apr23/14 - test option o# search occurrence# of pattern within record */
    if ((match) && (sargs.opi['o'-'a']))
      { sargs.pscd = mm;       /* init search2 start to 1st match +1 */
        ss = 1;                /* init search occurs ctr */
        while (ss < sargs.opi['o'-'a'])
          { ss++;              /* increment search repeat ctr */
            sargs.pscd = mm+1; /* increment search start dsplcmnt */
            mm = search2(recb,&sargs);
            if (sargs.pscd >= rsz1)
              { mm = -1;
                break;
              }
          }
      }

/*eject*/
/* test result of all 3 possible search conditions & possible AND/ORs  */
/* if match - store fileptr & match dsp for possible ss following      */
/*          - store match found msg & break search loop                */
/*          - opmsg1 will be displayed after found screen displayed     */
/* nomatch  - clear prior search match dsplcmnt & increment            */
/*          - advance fileptr to next record & repeat search loop      */
    if (match)
      { sargs.psfp = fileptr;
        sargs.pscd = mm;

        /* show found pattern as dsplcmnt in rec or file depending optn d4*/
        if (opsi['d'-'a'] & 0x04)
          { mmd = fileptr+mm;
            nnd = fileptr+nn;
            ood = fileptr+oo;
          }
        else
          { mmd = mm;
            nnd = nn;
            ood = oo;
          }
        /* if 3 search fields, report all 3 found dsplcmnts            */
        if (sargs.a6l)
          { sprintf(opmsg1,"found--> %s <--1st at %d, 2nd at %d, 3rd at %d\n",
                           sargs.cmd,mmd,nnd,ood);
          }
        /* if 2 search fields, report both found dsplcmnts             */
        else if (sargs.a4l)
          { sprintf(opmsg1,"found--> %s <--1st at byte# %d & 2nd at byte %d\n",
                           sargs.cmd,mmd,nnd);
          }
        else
          { sprintf(opmsg1,"found--> %s <--at byte# %d of record# %d\n"
                         ,sargs.cmd,mmd,recnum1);
          }

        /* ensure screen display will include found pattern      */
        /* calc rec index assume 320 bytes/screen                */
        rii = (mm/320) * 320;

        /* break while(1) loop on match & return +1              */
        /* But continue if option 'n' search < nth occurrence    */
        nth++;             /* increment search match occurrence# */
        if (nth >= sargs.opi['n'-'a'])
          { break;  
          }
        else
          { /* fileptr += rsz1;  May25/10 replace w getrec(0) */
            continue;
          }
      }

/*eject*/
    else /* nomatch */
      { /* fileptr += rsz1;  May25/10 replace w getrec(0) */
        sargs.pscd = 0;             /* ensure dsp extra clear        */
        rr++;                       /* count records searched        */
        if ((sargs.cmv) && (rr >= sargs.cmv))
          { sprintf(opmsg1,"NOfind--> %s within limit spcfd\n",sargs.cmd);
            break;
          }
        continue;
      }
  }
return(1);
}

/*eject*/
/*uvhd----------------------- search2 --------------------------------*/
/* search a record for a match, as specified in the CA structure      */
/* - see the CA structure defined earlier in this program             */
/* - defines search start, length, pattern data, length,& cc = > < !  */
/* if match   - return dsplcmnt to matching data                      */
/* if nomatch - return -1                                             */

int search2(char *recb, struct CA *cap)
{
int ii,jj,kk;                  /* misc indices                       */
int bb = cap->a1d;             /* dsplcmnt in rec to begin search    */
int ee = cap->a1d+cap->a1l;    /* dsplcmnt in rec to end search      */
int ll = cap->a2l;             /* length of search pattern           */

/* to allow search continuation within same record (see search1 rtn)  */
/* - use prior search dsplcmnt (+1) if specified                      */
if (cap->pscd)
  { bb = cap->pscd;
  }

/* use different search rtns, depending on search condition > < = !    */
/*  & depending on search pattern length - 1 byte or multi-bytes       */
/*  <!> 1 byte compares    - will compare to each byte of search area  */
/* <!> multi-byte compares - search area length must = pattern length  */
/*                        - will compare corresponding bytes           */
if (cap->a2cc[0] == '>')
 { if (ll == 1)
    { for (ii=bb; ii < ee; ii++)
        { if ((Uchar) recb[ii] > (Uchar) cap->a2c[0])
              return(ii);
        }
    }
   else
    { if (memcmp(&recb[bb],&(cap->a2c[0]),ll) > 0)
              return(bb);
    }
 }

else if (cap->a2cc[0] == '<')
 { if (ll == 1)
    { for (ii=bb; ii < ee; ii++)
        { if ((Uchar) recb[ii] < (Uchar) cap->a2c[0])
              return(ii);
        }
    }
   else
    { if (memcmp(&recb[bb],&(cap->a2c[0]),ll) < 0)
              return(bb);
    }
 }

/*eject*/
else if (cap->a2cc[0] == '!')
 { if (ll == 1)
     { /*Mar2003 - sep logic for != search in 1 byte OR multi-byte fields    */
       if (cap->a1l == 1)
         { for (ii=bb; ii < ee; ii++)
             { if ((Uchar) recb[ii] != (Uchar) cap->a2c[0])
                   return(ii);
             }
         }
       else
         { /*Mar2003 - Multi-byte field search for != 1 byte pattern        */
           /* count&save dsp of any matches, at end return dsp to last match*/
            { for (ii=bb,jj=0,kk=0; ii < ee; ii++)
                { if ((Uchar) recb[ii] == (Uchar) cap->a2c[0])
                    { kk++;               /* count matches                  */
                      jj = ii;            /* save dsp to last == in field   */
                    }
                }
              if (kk == 0)                /* if No matches in field         */
                  return(bb);             /* return dsp to 1st byte of field*/
              /*  return(jj); return dsp to last == Mar2003 Bad logic fixed */
            }
         }
     }
   else
     { if (memcmp(&recb[bb],&(cap->a2c[0]),ll) != 0)
           return(bb);
     }
 }

else       /* must be = condition */
 { for (ii=bb; ii < ee; ii++)
     { for (jj=ii,kk=0; jj < ee; jj++,kk++)
         { if (recb[jj] != cap->a2c[kk])
               break;
           if (kk >= (ll-1))
               return(ii);
         }
     }
 }
  return(-1);
}

/*eject*/
/*uvhd----------------------- search3 --------------------------------*/
/* search record for match to op4 constant, as specified in CA struct */
/* - see the CA structure defined earlier in this program             */
/* - defines search start, length, pattern data, length,& cc = > < !  */
/* if match   - return dsplcmnt to matching data                      */
/* if nomatch - return -1                                             */

int search3(char *recb, struct CA *cap)
{
int ii,jj,kk;                  /* misc indices                       */
int bb = cap->a3d;             /* dsplcmnt in rec to begin search    */
int ee = cap->a3d+cap->a3l;    /* dsplcmnt in rec to end search      */
int ll = cap->a4l;             /* length of search pattern           */

/* use different search rtns, depending on search condition > < = !    */
/*  & depending on search pattern length - 1 byte or multi-bytes       */
/*  <!> 1 byte compares    - will compare to each byte of search area  */
/* <!> multi-byte compares - search area length must = pattern length  */
/*                        - will compare corresponding bytes           */
if (cap->a4cc[0] == '>')
 { if (ll == 1)
    { for (ii=bb; ii < ee; ii++)
        { if ((Uchar) recb[ii] > (Uchar) cap->a4c[0])
              return(ii);
        }
    }
   else
    { if (memcmp(&recb[bb],&(cap->a4c[0]),ll) > 0)
              return(bb);
    }
 }

else if (cap->a4cc[0] == '<')
 { if (ll == 1)
    { for (ii=bb; ii < ee; ii++)
        { if ((Uchar) recb[ii] < (Uchar) cap->a4c[0])
              return(ii);
        }
    }
   else
    { if (memcmp(&recb[bb],&(cap->a4c[0]),ll) < 0)
              return(bb);
    }
 }

/*eject*/
else if (cap->a4cc[0] == '!')
 { if (ll == 1)
     { /*Mar2003 - sep logic for != search in 1 byte OR multi-byte fields    */
       if (cap->a3l == 1)
         { for (ii=bb; ii < ee; ii++)
             { if ((Uchar) recb[ii] != (Uchar) cap->a4c[0])
                   return(ii);
             }
         }
       else
         { /*Mar2003 - Multi-byte field search for != 1 byte pattern        */
           /* count&save dsp of any matches, at end return dsp to last match*/
            { for (ii=bb,jj=0,kk=0; ii < ee; ii++)
                { if ((Uchar) recb[ii] == (Uchar) cap->a4c[0])
                    { kk++;               /* count matches                  */
                      jj = ii;            /* save dsp to last == in field   */
                    }
                }
              if (kk == 0)                /* if No matches in field         */
                  return(bb);             /* return dsp to 1st byte of field*/
              /*  return(jj); return dsp to last == Mar2003 Bad logic fixed */
            }
         }
     }
   else
    { if (memcmp(&recb[bb],&(cap->a4c[0]),ll) != 0)
          return(bb);
    }
 }

else       /* must be = condition */
 { for (ii=bb; ii < ee; ii++)
     { for (jj=ii,kk=0; jj < ee; jj++,kk++)
         { if (recb[jj] != cap->a4c[kk])
               break;
           if (kk >= (ll-1))
               return(ii);
         }
     }
 }
  return(-1);
}

/*eject*/
/*uvhd----------------------- search4 --------------------------------*/
/* search record for match to op6 constant, as specified in CA struct */
/* - see the CA structure defined earlier in this program             */
/* - defines search start, length, pattern data, length,& cc = > < !  */
/* if match: return dsplcmnt to matching data, if nomatch: return -1  */
/*May2003 - op5 & op6 added to allow 3rd condition on search, etc     */

int search4(char *recb, struct CA *cap)
{
int ii,jj,kk;                  /* misc indices                       */
int bb = cap->a5d;             /* dsplcmnt in rec to begin search    */
int ee = cap->a5d+cap->a5l;    /* dsplcmnt in rec to end search      */
int ll = cap->a6l;             /* length of search pattern           */

/* use different search rtns, depending on search condition > < = !    */
/*  & depending on search pattern length - 1 byte or multi-bytes       */
/*  <!> 1 byte compares    - will compare to each byte of search area  */
/* <!> multi-byte compares - search area length must = pattern length  */
/*                        - will compare corresponding bytes           */
if (cap->a6cc[0] == '>')
 { if (ll == 1)
    { for (ii=bb; ii < ee; ii++)
        { if ((Uchar) recb[ii] > (Uchar) cap->a6c[0])
              return(ii);
        }
    }
   else
    { if (memcmp(&recb[bb],&(cap->a6c[0]),ll) > 0)
              return(bb);
    }
 }

else if (cap->a6cc[0] == '<')
 { if (ll == 1)
    { for (ii=bb; ii < ee; ii++)
        { if ((Uchar) recb[ii] < (Uchar) cap->a6c[0])
              return(ii);
        }
    }
   else
    { if (memcmp(&recb[bb],&(cap->a6c[0]),ll) < 0)
              return(bb);
    }
 }

/*eject*/
else if (cap->a6cc[0] == '!')
 { if (ll == 1)
     { /*Mar2003 - sep logic for != search in 1 byte OR multi-byte fields    */
       if (cap->a5l == 1)
         { for (ii=bb; ii < ee; ii++)
             { if ((Uchar) recb[ii] != (Uchar) cap->a6c[0])
                   return(ii);
             }
         }
       else
         { /*Mar2003 - Multi-byte field search for != 1 byte pattern        */
           /* count&save dsp of any matches, at end return dsp to last match*/
            { for (ii=bb,jj=0,kk=0; ii < ee; ii++)
                { if ((Uchar) recb[ii] == (Uchar) cap->a6c[0])
                    { kk++;               /* count matches                  */
                      jj = ii;            /* save dsp to last == in field   */
                    }
                }
              if (kk == 0)                /* if No matches in field         */
                  return(bb);             /* return dsp to 1st byte of field*/
              /*  return(jj); return dsp to last == Mar2003 Bad logic fixed */
            }
         }
     }
   else
    { if (memcmp(&recb[bb],&(cap->a6c[0]),ll) != 0)
          return(bb);
    }
 }

else       /* must be = condition */
 { for (ii=bb; ii < ee; ii++)
     { for (jj=ii,kk=0; jj < ee; jj++,kk++)
         { if (recb[jj] != cap->a6c[kk])
               break;
           if (kk >= (ll-1))
               return(ii);
         }
     }
 }
  return(-1);
}

/*eject*/
/*uvhd----------------------- search234 -------------------------------*/
/* search record for match to op3, op4,& op6 constants as per CA struct*/
/* - see the CA structure defined earlier in this program              */
/* - defines search start, length, pattern data, length,& cc = > < !   */
/* if match: return +1, if nomatch: return -1                          */
/*May2003 - subrtn renamed & extended to test results of search2,3,4   */

int search234(char *recb, struct CA *cap)
{
int mm,nn,oo;
mm = search2(recb,cap);             /* search for match on op2 constant*/

/* search for match on op4 constant if specified                      */
nn = 1;                             /* disable op4 consider if not spcfd*/
if (cap->a4l)
  { nn = search3(recb,cap);         /* search for match on op3 constant*/
  }

/* search for match on op6 constant if specified                      */
oo = 1;                             /* disable op6 consider if not spcfd*/
if (cap->a6l)
  { oo = search4(recb,cap);         /* search for match on op6 constant*/
  }

/* test results of 3 searches depending on conditions vs AND/ORs possible*/
/*May2003 - op4/op5 added to allow 3rd condition, also allowing AND/OR   */

if ((cap->a3cc[0] != '|') && (cap->a5cc[0] != '|'))
  { if ((mm >= 0) && (nn >= 0) && (oo >= 0))
        return(1);
  }
else if ((cap->a3cc[0] != '|') && (cap->a5cc[0] == '|'))
  { if ((mm >= 0) && ((nn >= 0) || (oo >= 0)))
        return(1);
  }
else if ((cap->a3cc[0] == '|') && (cap->a5cc[0] != '|'))
  { if ((mm >= 0) || ((nn >= 0) && (oo >= 0)))
        return(1);
  }
else if ((cap->a3cc[0] == '|') && (cap->a5cc[0] == '|'))
  { if ((mm >= 0) || (nn >= 0) || (oo >= 0))
        return(1);
  }

return(-1);
}

/*eject*/
/*uvhd------------------------ search34 -------------------------------*/
/* search record for match to op3/op4,& op5/op6 constants per CA struct*/
/* - see the CA structure defined earlier in this program              */
/* - defines search start, length, pattern data, length,& cc = > < !   */
/* if match: return +1, if nomatch: return -1                          */
/*May2003 - op4/op5 3rd condition added                                */
/* search234 used for all 3 conditions (search,write,enumerate,print)*/
/* search34 used to test 2nd & 3rd conditions (update)                 */
/* - since op1/op2 (1st condition) used for update constant            */

int search34(char *recb, struct CA *cap)
{
int nn,oo;

/* search for match on op4 constant if specified                      */
nn = 1;                             /* disable op4 consider if not spcfd*/
if (cap->a4l)
  { nn = search3(recb,cap);         /* search for match on op3 constant*/
  }

/* search for match on op6 constant if specified                      */
oo = 1;                             /* disable op6 consider if not spcfd*/
if (cap->a6l)
  { oo = search4(recb,cap);         /* search for match on op6 constant*/
  }

/* test results of 2 searches depending on conditions vs AND/OR possible*/

if (cap->a5cc[0] == '|')
  { if ((nn >= 0) || (oo >= 0))
        return(1);
  }
else
  { if ((nn >= 0) && (oo >= 0))
        return(1);
  }

return(-1);
}

/*eject*/
/*uvhd------------------------ print1 ---------------------------------*/
/* print specified number of records                                   */
/* - or records matching a pattern, to a specified maximum             */
/*                                                                     */
/* p5                        - print next 5 records                    */
/* p5l100                    - print 5 recs line width 100 bytes       */
/*                                                                     */
/* p5 0(20),='ABC'           - search for & print records with 'ABC'   */
/*                             within 1st 20 bytes to max 5 recs       */
/*                                                                     */
/* pp                        - repeat previous search                  */
/*                                                                     */
/* - if nofind set fileptr to last rec in file & return 0              */

int print1(int lsize)
{
int nn;         /* max records search ctr       */
int mm;         /* search find/nofind 0+/-      */
int ff;         /* calc rmndr formfeed option f */
int ss;         /* space option p or cmd line s */
upnext = 0;    /* inhibit advance to next record on 1st getrec */

/* test for 'p=s' transfer last search cmd to print cmd storage structure */
if ((cms[1] == '=') && (cms[2] == 's'))
  { pargs = sargs;              /* store print cmd from last search cmd */
    pargs.cmd[0] = 'p';         /* change cmd from 's' to 'p'           */
    pargs.cmc[0] = 'p';         /* change cmc from 's' to 'p'           */
    printf("print args stored from last search: %s \n",pargs.cmd);
    printf("execute 'pp' to print spcfd records from current to EOF --> ");
    return(2);
  }

/* test for new print command or repeat previous print command (pp)  */
if (cms[1] == 'p')                      /* repeat prior print ?      */
  { /* allow value updt on repeat cmd - pp99                         */
    if (cargs.cmv)
        pargs.cmv = cargs.cmv;
  }
else
  { /* store new search args - but 1st verify (cargs here, pargs below)*/
    if ((cargs.cmv == 0) && (cargs.a2l == 0))
      { errmsg("print cmd must specify number or pattern",cargs.cmd,"",0,0x10);
        return(0);
      }
    pargs = cargs;                      /* store new print args      */
  }

/* store space option for printrec from s option on command line     */
/* - but override if s option spcfd on print command                 */
if (pargs.opc['s'-'a'])
  { ss = pargs.opi['s'-'a'];
  }
else
  { ss = opsi['s'-'a'];
  }

/*eject*/
/* verify print a number of records or search pattern           */
/* - repeat here for pargs vs cargs in case transfer for search */
if ((pargs.cmv == 0) && (pargs.a2l == 0))
  { errmsg("print cmd must specify number or pattern",pargs.cmd,"",0,0x10);
    return(0);
  }

/*Aug13/11 - combine iprint into print as option 'i' */
/*         - ensure any prior print file closed      */
if (copsc['i'-'a'])
  { /*Feb05/09 - close file on each print (vs all prints to same file) */
    closefileX(pfname,"P",&pfptr,&pfopn,&pfctr,0x00);
  }

/*Feb05/09 - call function to open file on each print */
openfileX(pfname,"P",&pfptr,&pfopn,0x00);

fprintf(pfptr,"now=%s uvhd %s %s\n",todttm,fn1a,opsu);
fprintf(pfptr,"version=%s lastmod=%s print=%s\n",uvversion,fmdate,pargs.cmd);

/*May27/10 - if at begin file show filesize/recsize */
if (recnum1 <= 1)
  { fprintf(pfptr,"records=%d %s rsize=%d fsize=%s%s\n"
                 ,rcount,dcounts2,rsz1,fsedit,rmndrs);
  }

/* modify line-width if option l spcfd (default 100 or cmd line optn l)*/
if (pargs.opi['l'-'a'])
    lsize = pargs.opi['l'-'a'];

/*eject*/
/* print spcfd no of recs, or search by pattern & print to max recs  */
nn=0; fpsv=0; rnsv=0;                  /* reset max print ctr        */
while (1)
 { rsz1 = getrec(0);
   if (rsz1 <= 0)                      /* EOF ?                       */
     { break;
     }

   /* if search pattern spcfd - search for & print matching recs to max cnt*/
   if (pargs.a2l)
     { mm = search234(recb,&pargs);   /* 9701 search = > < !             */
       if (mm >= 0)
         { printrec(pfptr,lsize,ss);  /* print matching record           */
           nn++;                      /* count records printed (max test)*/
           pfctr++;                   /* count prints for stats          */
           fpsv = fileptr;            /* save fileptr of last matched rec*/
           rnsv = recnum1;            /* save recnum1 last rec matched   */
         }
     }
   else
     { printrec(pfptr,lsize,ss);      /* print current record            */
       nn++;                          /* count records printed (max test)*/
       pfctr++;                       /* count prints for stats          */
       fpsv = fileptr;                /* save fileptr of last matched rec*/
       rnsv = recnum1;                /* save recnum1 last rec matched   */
     }

   /* test max print count reached, if so display confirmation & return  */
   if ((pargs.cmv) && (nn >= pargs.cmv))
     { break;
     }

   /* if option f, write formfeed every f# records                  */
   if (copsi['f'-'a'])
     { ff = nn % copsi['f'-'a'];
       if (ff == 0)
          fprintf(pfptr,"\f");
     }

   /* increment fileptr to next record & repeat loop */
   /* fileptr += rsz1;  May25/10 replace w getrec(0) */
 }

/*eject*/
/*Mar19/19 - also print any stored oppmsgs (as well as display on screen) */
/*     - endscreen: copies opmsg1,2,3 to oppmsg1,2,3 for folwng printrec()*/
/* print any stored oppmsg & clear to prevent repeat                      */
if ((oppmsg1[0]) || (oppmsg2[0]) || (oppmsg3[0]))
  { if (oppmsg1[0])
      { fprintf(pfptr,"%s",oppmsg1);
        strcpy(oppmsg1,oppmsg1);  /*Mar19/19 - save for any folwng printrec*/
      }
    if (oppmsg2[0])
      { fprintf(pfptr,"%s",oppmsg2);
        strcpy(oppmsg2,oppmsg2);
      }
    if (oppmsg3[0])
      { fprintf(pfptr,"%s",oppmsg3);
        strcpy(oppmsg3,oppmsg3);
      }
    oppmsg1[0] = '\0';
    oppmsg2[0] = '\0';
    oppmsg3[0] = '\0';
    /* - generate divider line to empahsize preceding errmsgs */
    fprintf(pfptr,"--------------------------------------------------------------\n");
  }

/*Feb05/09 - close file on each print (vs all prints to same file) */
/*Dec29/10 - move close print file to EOJ, lp cmd added to close   */
/* closefileX(pfname,"P",&pfptr,&pfopn,&pfctr,0x00); */

/* print loop ended by max count, or EOF + errmsg if no recs printed */
sprintf(opmsg1,"%s --> %d recs printed %s\n",pargs.cmd,nn,pfname);

/*eject*/
/*Aug15/11 - iprint changed to option 'i' on print command */
if (copsc['i'-'a'])
  { /*Feb05/09 - close file on each print (vs all prints to same file) */
    closefileX(pfname,"P",&pfptr,&pfopn,&pfctr,0x00);

    /* iprint cmd built at prgm init Portrait or Landscape if l optn > 68  */
    if (iargs.opi['l'-'a'] > 68)
      { strcpy(prtcmdWfile,prtcmdW);       /* setup uvlp12L/lp for iprint LS  */
        strcat(prtcmdWfile," ");
        strcat(prtcmdWfile,pfname);        /* append iprint filename          */
        system(prtcmdWfile);         /* execute uvlp12L cmd (built at init) */
      }
    else
      { strcpy(prtcmdfile,prtcmd);         /* setup uvlp/lp for iprint cmd   */
        strcat(prtcmdfile," ");
        strcat(prtcmdfile,pfname);         /* append iprint filename         */
        system(prtcmdfile);          /* execute uvlp12 cmd (built at init) */
      }
  }
else
  { /*Dec29/10 - add 2nd msg re new cmd 'lp' to close printfile if desired*/
    sprintf(opmsg3,
     "use 'lp'(close print) for separate files, omit for combined files\n");
  }

if (nn == 0)
  { printf("EOF reached with NO records printed \n");
    return(0);
  }
else
  { fileptr = fpsv;  /* so last printed rec will be displayed     */
    recnum1 = rnsv;  /*Jun03/10 also set recnum1 last matched     */
    upnext = 0;      /* inhibit getrec advance to display correct */
    if (fileptr >= fileptr1)
      { sprintf(opmsg2,"searched to EOF & reset to last record printed\n");
      }
  }
return(1);
}

/*Aug15/11 - iprint changed to option 'i' on print command */

/*eject*/
/*uvhd------------------------ printrec -------------------------------*/
/* print current record to the output print file                       */
/* - not used for screen display (which must not exceed screen size)   */
/* - this subrtn will write formatted lines to the print file          */
/*   for the entire record length                                      */

int printrec(FILE *fptr,int lsize, int pspace)
{
/*Oct06/10 - print record & file info prior to scale & data */
/*Oct08/16 - add deleted count to displays & Index file save/load */
fprintf(fptr,"rec#=%d rsize=%d fptr=%s fsize=%s records=%d %s\n"
            ,recnum1,rsz1,fpedit1,fsedit,rcount,dcounts2);

/* set scalesize = recsize if scalesize > recsize */
lszh = lszo;
if (lszh > rsz1)
  { lszh = rsz1;
  }
  /*Mar08/2022 - set scale size 2 bytes extra if file type z16 (z2+z16)*/
  /*Mar11/2022 - now NOT Required after further testing */
  /* if (opsi['z'-'a'] & 0x10) */
  /*   { lszh +=2;             */
  /*   }                       */

/* output screen hdr record column scale, 0/1 relative by option g0/g1 */
sncopy(scalet2,scalet1,lszh,0x03);  /* copy & truncate tens scale lszh */
sncopy(scaleu2,scaleu1,lszh,0x03);  /* copy & truncate units scale lszh*/

/*May2001 - allow 32/64 bit fileptr, insert at begin scale1              */
/*        - edit fileptr rt justified in 8 or 11 bytes depending on value*/
/*Apr05/07 - test option o# to display scale every # records */
/*May15/07 - option changed from 'i' to 'o'                  */
/* calc remainder (recnum1 % optn o) only if optn o spcfd    */
recnum1o = 1;     /* presume no option o for test below      */
if ((opsi['o'-'a']) && (recnum1 > 1))
  { recnum1o = (recnum1 % opsi['o'-'a']);
  }
if (recnum1o == 1)
  { if (pspace & 0x01)           /* space before scale ? */
      { fprintf(fptr,"\n");
      }
    /* output screen hdr record column scale, 0/1 relative by option g0/g1 */
    fprintf(fptr,"            %s\n",scalet2);   /* tens scale fileptr removed*/
    fprintf(fptr,"r#%9d %s\n",recnum1,scaleu2); /* units scale              */
    linctr += 2;                           /* incrmnt line ctr for scale   */
    if (pspace & 0x08)           /* space after scale ? */
      { fprintf(fptr,"\n");
      }
  }
else
  {  fprintf(fptr,"\n");     /* space 1 vs print scale */
     linctr++;
  }

/*eject*/
/* begin loop to print 3 part vert hex lines, until end of record      */
/* getlinr - extracts next display segment from current record         */
while(1)
  { lsz1 = getlinr(lsize);
    if (lsz1 <= 0)
      { break;
      }
    memcpy(linc,linr,lsz1);          /* copy for translate/filter (LMAX)*/

    /* test option 'a' to translate character line from EBCDIC to ASCII */
    /* note - bit 0x04 converts non-printable EBCDIC chars to periods   */
    if (opsc['a'-'a'])                   /* option 'a' active ?         */
       toascii2((Uchar*)linc,lsz1,0x04); /* translate EBCDIC to ASCII (LMAX)*/

    /*Apr11/10 - test option to drop blank lines */
    /*Sep11/15 - option d1 also inhibit null segments */
    if ((opsi['d'-'a'] & 0x01) && (rii1))
      { if (memcmp(linc,blank256,lsz1) == 0)
          { continue;
          }
        if (memcmp(linr,null256,lsz1) == 0)
          { continue;
          }
      }
    /* now convert data line to 3 lines for vertical hex display        */
    /* - presuming full 64 bytes per line (will compensate below)       */
    filterchar(linc,lsz1);           /* format line #1 - characters     */
    hexzone(linz,linr,lsz1);         /* format line #2 - hex zones      */
    hexdigit(lind,linr,lsz1);        /* format line #3 - hex digits     */

    /* now insert the array nulls again - based on length returned by   */
    /* getlinr which may be < 64 if optn 'l' or optn 'f' or eof         */
    linc[lsz1] = '\0';
    linz[lsz1] = '\0';
    lind[lsz1] = '\0';

    /* fprintf(fptr,"  %8ld %s\n",rii1,linc);                             */
    /*Nov2001 - fileptr removed on tens scale, now insert on data segments*/
    /*Dec2001 - always display fileptr on 1st segment of record           */
    /*     d0 - display record dsplcmnts on following segments of record  */
    /*     d4 - display file dsplcmnts on all segments of record          */
    /*May27/10 - default record dsplcmnts on all segments                 */
    /*     d4 - show file dsplcmnts on all segments                       */

    if (pspace & 0x02)           /* test option to space before data lines */
      { fprintf(fptr,"\n");
      }

/*eject*/
    /*Jun05/10 - test option d4 - show file-dsp vs rec-dsp on all segments */
    /*Oct08/11 - edit fileptr for printf's %ld or %lld for 64/32bit HP gcc */
    if (rii1 == 0)
      { sprintf(fpedit2,E64,fileptr);          /* edit allowing 32/64 bits */
      }
    else
      { if (opsi['d'-'a'] & 0x04)
          { sprintf(fpedit2,E64,fileptr+rii1); /* file dsplcmnts on all segs*/
          }
        else
          { sprintf(fpedit2,E64,rii1);         /* record dsplmnts on segs 2+*/
          }
      }
    fprintf(fptr,"%11s %s\n",fpedit2,linc);

    /* display by option 'h' & any < 0x20 or > 0x7E not \n \r \t \f    */
    /*Nov11/11 - change options */
    /* h0 = hex (default), h1 = force chars only, h2 = auto determine by data */
    if (opsi['h'-'a'] & 0x01)
      { ; }
    /* else if ((opsi['h'-'a'] & 0x02) && (xchar < 1)) */
    /*Mar04/13 - fix h2 cmd not forcing hex - remove (xchar < 1) */
    else
      { fprintf(fptr,"            %s\n",linz);
        fprintf(fptr,"            %s\n",lind);
      }
  } /* end while loop to print record 3 line hex groups */

/* test option for extra space between records when printing */
if (pspace & 0x04)          /* space 3 option ?           */
  { fprintf(fptr,"\n");
  }
return(1);
}

/*eject*/
/*uvhd------------------------ write1 ---------------------------------*/
/* write specified number of records                                   */
/* - or records matching a pattern, to a specified maximum             */
/*                                                                     */
/* w5                        - write next 5 records                    */
/*                                                                     */
/* w5e4x 0(20),='ABC'        - search for & write records with 'ABC'   */
/*                             within 1st 20 bytes to max 5 recs       */
/*                           - write Every 4th record (option e4)      */
/*                           - write other records to 2nd write file   */
/*                             (tmp/filename.datetimeX vs W)           */
/*                                                                     */
/* ww                        - repeat previous write                   */
/*                                                                     */
/* - if nofind set fileptr to last rec in file & return 0              */

int write1(void)
{
int nn;         /* max records search ctr        */
int mm;         /* search find/nofind 0+/-       */
int ii;         /* recs dropped by DISAM flag    */
int tt;         /* count for every nth select    */
int ww;         /* set if current record written */
int xx;         /* count non-matched recs written*/
int yy;         /* count non-matched recs til EOF*/
upnext = 0;     /* inhibit advance to next record on 1st getrec */

/* test for 'w=s' transfer last search cmd to write cmd storage structure */
if ((cms[1] == '=') && (cms[2] == 's'))
  { wargs = sargs;              /* store write cmd from last search cmd */
    wargs.cmd[0] = 'w';         /* change cmd from 's' to 'w'           */
    wargs.cmc[0] = 'w';         /* change cmc from 's' to 'w'           */
    printf("write args stored from last search: %s \n",wargs.cmd);
    printf("execute 'ww' to write spcfd records from current to EOF --> ");
    return(2);
  }

/* test for new write command or repeat previous write command (ww)  */
if (cms[1] == 'w')                      /* repeat prior write ?      */
  { /* allow value updt on repeat cmd - ww99                         */
    if (cargs.cmv)
        wargs.cmv = cargs.cmv;
  }
else
  { /* store new search args - but 1st verify (cargs here, pargs below)*/
    if ((cargs.cmv == 0) && (cargs.a2l == 0))
      { errmsg("write cmd must specify number or pattern",cargs.cmd,"",0,0x10);
        return(0);
      }
    wargs = cargs;                      /* store new write args      */
  }

/*eject*/
/* verify write a number of records or search pattern */
if ((wargs.cmv == 0) && (wargs.a2l == 0))
  { errmsg("write cmd must specify number or pattern",cargs.cmd,"",0,0x10);
    return(0);
  }

/*Feb05/09 - common functions openfileX & closefileX for P,I,W,V,D files*/
/*  - chg P/I/W/V/D cmds to open & close output file on each cmd */
/*  - cmd line option w1 to write 1 combined file REMOVED 2010   */
/*  - filename format as follows:                                */
/* $UVTMPDIR/inputfilename_yymmdd_hhmmssX               <-- X=P/I/W/V/D */
/* $UVTMPDIR/inputfilename_yymmdd_ <-- basefilename + time + X each open*/

/*Feb05/09 - call function to open file on 1st write (depending optn w1)*/
openfileX(wfname,"W",&wfptr,&wfopn,0x00);

/* write spcfd no of recs, or search by pattern & write to max recs  */
nn=0; ii=0; fpsv=0; rnsv=0; tt=0; xx=0; yy=0;  /* clear counters     */

/*Feb26/19 - if w99, reset fileptr=0 (parse already set count 999999999) */
if (wargs.cmv == 999999999)
  { fileptr = 0;
    fileseek("write w99");
  }

/*Mar19/19 - default output record terminator to LF (option t2)     */
/*  - if variable lth (ftype != 'f') & no options 't#' or 'z' spcfd */
if ((ftype != 'f') &&  (wargs.opi['t'-'a'] == 0) &&  (wargs.opi['z'-'a'] == 0))
  { wargs.opi['t'-'a'] = 2;
  }

/*eject*/
/* begin loop to select records & write via writerec() */
while (1)
 { rsz1 = getrec(0);
   if (rsz1 <= 0)                       /* EOF ?                       */
     { break;
     }
   ww = 0;                            /* reset switch cur rec written   */

   /* if search pattern spcfd - search for & write matching recs to max cnt*/
   if (wargs.a2l)
    { mm = search234(recb,&wargs);   /* 9701 search = > < !            */
      if (mm >= 0)
       { /* test option s to select every nth record                    */
         tt++;                       /* count all matches for every nth test*/
         if (wargs.opi['s'-'a'])
          { if ((tt % wargs.opi['s'-'a']) == 0)
             { writerec();           /* write matching record           */
               nn++;                 /* count records written (max test)*/
               ww++;                 /* set sw cur rec written          */
               fpsv = fileptr;       /* save fileptr of last matched rec*/
               rnsv = recnum1;       /* save recnum1 last rec matched   */
             }
          }
         else
          { writerec();              /* write matching record           */
            nn++;                    /* count records written (max test)*/
            ww++;                    /* set sw cur rec written          */
            fpsv = fileptr;          /* save fileptr of last matched rec*/
            rnsv = recnum1;          /* save recnum1 last rec matched   */
          }
       }
    }
/*eject*/
   else
    { /* test option e to select every nth record */
      tt++;                          /* count all matches for every nth test*/
      if (wargs.opi['s'-'a'])
       { if ((tt % wargs.opi['s'-'a']) == 0)
          { writerec();              /* write matching record           */
            nn++;                    /* count records written (max test)*/
            ww++;                    /* set sw cur rec written          */
            fpsv = fileptr;          /* save fileptr of last matched rec*/
            rnsv = recnum1;          /* save recnum1 last rec matched   */
          }
       }
      else
       { writerec();                 /* write current record            */
         nn++;                       /* count records written (max test)*/
         ww++;                       /* set sw cur rec written          */
         fpsv = fileptr;             /* save fileptr of last written rec*/
         rnsv = recnum1;             /* save recnum1 last rec matched   */
       }
    }

   /* increment fileptr to next record & repeat loop  */
   /* fileptr += rsz1;  May25/10 replace w getrec(0) */

   /* test max write count reached, if so display confirmation & return */
   if ((wargs.cmv) && (nn >= wargs.cmv))
     { break;
     }
 }

sprintf(opmsg1,"%s %d records written to: %s\n",wargs.cmd,nn,wfname);
sprintf(opmsg2,"searched to EOF & reset to last record written\n");

/*Dec29/10 - add 2nd msg re new cmd 'lp' to close printfile if desired*/
sprintf(opmsg3,
        "- use 'lw' (close write) for separate files, omit for combined\n");

/*eject*/
if (nn == 0)
  { printf("EOF reached with NO records written \n");
    return(0);
  }
else
  { fileptr = fpsv;       /* so last written rec will be displayed     */
    recnum1 = rnsv;       /*Jun03/10 also set recnum1 last matched     */
    upnext = 0;           /* inhibit getrec advance to show correct rec*/
  }

return(1);
}

/*eject*/
/*uvhd------------------------ writerec --------------------------------*/
/* write current record to output file                                  */

int writerec(void)
{
int ii;
int ss;
int nn;
int dd;         /*Nov23/10 - option d dsplcmnt to output data          */
int rr;         /*Nov23/10 - recsize to write when option d spcfd      */
                /*         = option 'r' spcfd, else input recsize - dd */
char wrsc[8];   /*Nov08/07 recsize numeric chars insert by option n1/n2*/

/* test options n1/n2 for 4 byte numeric recsize at begin/end data record */
wrsn1 = 0; wrsn2 = 0;
if (wargs.opi['n'-'a'] & 0x01)
  { wrsn1 = 4;
  }
else if (wargs.opi['n'-'a'] & 0x02)
  { wrsn2 = 4;
  }
wrsn3 = wrsn1 + wrsn2;  /* combine (4 or 0) for easier calcs below */

/*Feb07/09 - calc actual data size depending on input file type     */
/* - OK to subtract hdr sizes for both RDW & IDX (will be 0 if n/u) */
rsz1d = (rsz1 - rdwhs - vrhs);

/* determine write rcsz - option r if spcfd, else (current-rcsz - dd)  */
/*Nov25/10 - if no optn r, write size = cur-rcsz - dd (dd 0 if unspcfd)*/ 
dd = wargs.opi['d'-'a'];             /* capture optn d for convenience */
if (wargs.opi['r'-'a'])
  { wrs1w = wargs.opi['r'-'a'];
  }
else
  { wrs1w = (rsz1d - dd);            /* presume I/O write = data*/
    if (wrs1w < 0)
      { wrs1w = 4;
      }
  }

/*eject*/
/* test write options z2/z4 for RDW output            */
/* - set prefix size & store prefix in outrec area    */
/* - memcpy(wrecb,u2.cc,2);  - see dup 50 lines ahead */
/*Mar04/2022 - add option z16 for z2 files to INCLUDE pprefix (z2+z16=z18)*/
wrs1h = rsz1d;                       /* presume size in hdr = data-size   */
if (wargs.opi['z'-'a'] & 0x02)
  { wrhs = 2;                        /* store RDW hdr size      */
    wrs1w += (2 + wrsn3);            /* I/O write size          */
    wrs1h += (0 + wrsn3);            /* z2  value in hdr EXCLUDING prefix */
    if (wargs.opi['z'-'a'] & 0x10)
      { wrs1h += (2 + wrsn3);        /* z18 value in hdr INCLUDING prefix */
      }
  }
else if (wargs.opi['z'-'a'] & 0x0C)  /*Jan14/11 - z4 or z8      */
  { wrhs = 4;                        /* store RDW hdr size      */
    wrs1w += (4 + wrsn3);            /* I/O write size          */
    wrs1h += (4 + wrsn3);            /* value in RDW hdr same   */

 /* Mar03/2022 - Question re wrs1h for z16 ?   */
 /* wrs1h += (0 + wrsn3);   <-- why += (0 ... for z2 */ 
 /* wrs1h += (4 + wrsn3);   <-- why += (4 ... for z4 */

 /*Mar03/2022 - option z16 RDW files hdrsize=total recsize vs datasize   */
 /* z16 - subtract hdrsize in getrdwh, existing assumes hdrsize=datasize */
 /*     - write to add hdrsize, existing logic assuming hdr=datasize     */
 /*     - write logic yet to do as of Mar03/2022                         */
  }
else
  { wrhs = 0;
  }

/* create recsize prefixes in case of options z2/z4 with possible z1 */
/* - allow for BIG-end/littel-end machine                            */
/* - allow for option z1 to create little end format                 */
wrs1hs = wrs1h;                 /* move int recsize to short */
memset(u1.cc,'\0',4);           /* init union to switch ends */
memcpy(u1.cc,(char*)&wrs1hs,2); /* copy short into union     */
memcpy(u2.cc,u1.cc,4);          /* copy to output in case no switch req'd*/

/* if BEM - we are done (unlikely to use option z1) */
/* if LEM & option z1 we are done */
/* if LEM & not option z1, we need to switch ends   */
#if (LEM)
if (!(wargs.opi['z'-'a'] & 0x01))
  { u2.cc[0] = u1.cc[1];
    u2.cc[1] = u1.cc[0];
  }
#endif

/*eject*/
/* calc input record data adrs = base adrs + any IDX or RDW prefix size */
irecd = (recb + vrhs + rdwhs);

/* calc adrs for out record data = base + z2/z4 offset + n1 offset */
wrecd = (wrecb + wrhs + wrsn1);

/* clear wrec area to allow for variable length records  */
/* - clear min 2048 or rsz1 if larger                    */
if (rsz1 > 2048)
  { nn = rsz1; }
else
  { nn = 2048; }
/*Nov08/07 - clear to ASCII blank default, but EBCDIC blank if option a */
if (opsc['a'-'a'])
  { memset(wrecd,0x40,nn);       /* clear write area to EBCDIC blanks */
  }
else
  { memset(wrecd,0x20,nn);       /* clear write area to ASCII blanks  */
  }

/* store RDW hdr if any */
if (wargs.opi['z'-'a'] & 0x02)
  { memcpy(wrecb,u2.cc,2);
  }
else if (wargs.opi['z'-'a'] & 0x0C)  /*Jan14/11 - z4 or z8 */
  { memcpy(wrecb,u2.cc,4);
  }

/*Jan14/11 - allow option z8 write BDW = RDW+4 (on every record) */
if (wargs.opi['z'-'a'] & 0x08)
  { u3 = u1;
    u3.ss += 4;                    /* BDW size = RDW size + 4 */
    u4 = u3;                       /* done if BEM */
#if (LEM)
    if (!(wargs.opi['z'-'a'] & 0x01))
      { u4.cc[0] = u3.cc[1];
        u4.cc[1] = u3.cc[0];
      }
#endif
    ss = fwrite(u4.cc,1,4,wfptr);  /* write BDW */
    if (ss <= 0)
      { errmsg("writing BDW to output file",wargs.cmd,wfname,0,0x40);
      }
  }

/*Mar03//2022 - ignoring z16 option for BDW files */

/*eject*/
/*Aug17/10 - if input file Text, remove any trailing CR/LF */
if (ftype == 't')
  { for (ii = rsz1d; ii >= 0; ii--)
      { if (irecd[ii] == 0x0A)
          { irecd[ii] = ' ';
            if ((ii > 0) && (irecd[ii-1] == 0x0D))
              { irecd[ii-1] = ' ';
              }
            break;
          }
      }
  }

/* copy input data to write output area                     */
/*Feb07/09 - copy just the data size from input record      */
/*Nov25/10 - allow option 'd' dsplcmnt of 1st byte to write */
memcpy(wrecd,irecd+dd,rsz1d);

/*Nov08/07 - test option a1 to translate write records to ASCII      */
/*Apr16/08 (Uchar*) inserted for HP compile warning mismatch ptr types*/
if (wargs.opi['a'-'a'] & 0x01)
  { toascii2((Uchar*)wrecd,wrs1w,0x00);
  }

/*Aug17/10 - test option e1 to translate write records to EBCDIC */
if (wargs.opi['e'-'a'] & 0x01)
  { toebcdic2((Uchar*)wrecd,wrs1w,0x00);
  }

/* translate any unprintable chars to '.' periods if option c1 */
if (wargs.opi['c'-'a'] & 0x01)
  { toprint2((Uchar*)wrecd,wrs1w,0x00);
  }

/*Nov08/07 - insert recsize as 4 numeric characters if option n1/n2     */
/*           (n1 = insert at begin record, n2 = insert at end record)   */
/*Feb09/09 - fix write optn n1/n2, insert input size, not optn r outsize*/
/*         - assume user wants input data & not any optn r fixed size   */

/* calc adrs to insert option n1 numeric recsize (if any) */
wrecn1 = (wrecb + wrhs);

/* calc adrs for option n2 numeric recsize (if any) */
wrecn2 = (wrecb + wrs1w - 4);

sprintf(wrsc,"%04d",rsz1d);   /* convert input data size to digits */

/* if option n1/n2 - insert write size at begin/end record */
if (wargs.opi['n'-'a'] & 0x01)
  { memcpy(wrecn1,wrsc,4); 
  }
else if (wargs.opi['n'-'a'] & 0x02)
  { memcpy(wrecn2,wrsc,4);
  }

/*eject*/
/* if option t4 - append CR/LF terminator after last nonblank     */
/*Feb09/09 - fix write optn t4, scan back not writing shorter size*/
if (wargs.opi['t'-'a'] & 0x04)
  { for (ii=wrs1w-1; ii > 8; ii--)
      { if ((Uchar)wrecb[ii] > 0x20)
          { break;
          }
      }
    if (wargs.opi['t'-'a'] & 0x01)
      { wrecb[++ii] = '\r';
      }
    if (wargs.opi['t'-'a'] & 0x02)
      { wrecb[++ii] = '\n';
      }
    ii++;             /*Aug17/10 - fix size here (not in both above ifs*/
    wrs1w = ii;       /*Feb09/09 - write size ending at LineFeed       */
  }
else /* not t4 after LNB - append CR/LF at end rec & increase recsz */
  {  /*Dec11/07 - ensure fixed size = optn w independent optns p,k  */
     /*         - but terminators extra depending optn t1,t2,t3     */
     /*Feb05/09 - terminator chgd from addon to within recsize      */
     /*May19/10 - change terminator back to following recsize       */
     /*         - losing data if input is RDW                       */
    if ((wargs.opi['t'-'a'] & 0x01) && (wargs.opi['t'-'a'] & 0x02))
      { wrecb[wrs1w] = '\r';
        wrecb[wrs1w+1] = '\n';
        wrs1w += 2;
      }
    else if (wargs.opi['t'-'a'] & 0x01)
      { wrecb[wrs1w] = '\r';
        wrs1w++;
      }
    else if (wargs.opi['t'-'a'] & 0x02)
      { wrecb[wrs1w] = '\n';
        wrs1w++;
      }
  }

ss = fwrite(wrecb,1,wrs1w,wfptr);   /* write complete record */
if (ss <= 0)
  { errmsg("writing output file",wargs.cmd,wfname,0,0x40);
  }
wfctr++;                          /* count writes */

return(1);
}

/*eject*/
/*uvhd------------------------- update --------------------------------*/
/* update current record - return 1 if ok, 0 if any error              */
/*                   - sample commands follow:                         */
/*  u 0(3),='ABC'    - change 1st 3 bytes of current record to 'ABC'   */
/*  u 10(3),=x'0D0A' - change bytes 11&12 of current record to x'0D0A' */
/*  uu               - repeat prior update (on current record)         */

int update(void)
{
int uu;         /* records updated counter          */
int nn;         /* records read searching for updts */
int ss;         /* op3/4 & op4/5 qualify status     */
int ll;         /* excess lth to clear if op1 lth > op2 lth */
char xlc;       /* eXtra Lth Clear char - blank or null if hex constant */

upnext = 0;     /* inhibit advance to next record on 1st getrec */
uu=0; nn=0;     /* clear counters */
fpsv=0; rnsv=0; 

/* verify option u to allow update */
if ((opsc['u'-'a']) == 0)
  { printf(">>>UPDATE requires option u on uvhd command line<<<\n");
    showhelp(help07,26,0x01);
    printf(">>>UPDATE requires option u on uvhd command line<<<\n");
    exit(9);
  }

/* save record for possible roll-back of last update to current record */
memcpy(recsav2,recb,rsz1);        /* save record for possible roll-back*/

/* save record for possible roll-back of multi updts to current record */
/* IE - save only if cur fileptr not = fileptr of last updt            */
if (fileptr != fplastup)
    memcpy(recsav1,recb,rsz1);   /* save rec for multi updt roll-back  */

/* test for new update or repeat previous update                      */
if (cms[1] == 'u')                      /* repeat prior update ?      */
  { /* trsfr any multi-rec count from uu99 to stored uargs structure  */
    /* - to allow user to test update on 1 record, then repeat on all */
    uargs.cmv = cargs.cmv;              /* update multi-rec count     */
  }
else
  { /* store new search args - but 1st verify (cargs here, uargs below)*/
    if ((cargs.a1l <= 0) || (cargs.a2l <= 0))
      { errmsg("update args invalid",cargs.cmd,"",0,0x10);
        return(0);
      }
    uargs = cargs;                 /* store new search args */
  }

/*May20/13 - set extra lth clear char null, if hex constant, else AEB */
if (uargs.a2cc[1] == 'x')
  { xlc = '\0'; }
else
  { xlc = AEB; }

/*eject*/
/* verify update args - op1 lth & op2 constant lth > 0 ? */
/* - need repeat here in case transfer from search args  */
if ((uargs.a1l <= 0) || (uargs.a2l <= 0))
  { errmsg("update args invalid",uargs.cmd,"",0,0x10);
    return(0);
  }

/*Dec07/10 - verify update op1 dsp+lth not > recsize */
if ((uargs.a1d + uargs.a1l) > rsz1)
  { errmsg("update op1 dsplcmnt+length > recsize",uargs.cmd,"",0,0x10);
    return(0);
  }

/*Jun04/10 - test option to transfer update args to search args */
/*         - for subsequent 'ss' to verify updates              */
if (uargs.opc['s'-'a'])
  { sargs = uargs;       /* transfer updt args to search args */
    sargs.a4l = 0;       /* disable search on op4,5,6         */
    sargs.a5l = 0;   
    sargs.a6l = 0;
  }

/* instruction verified - test for multi-rec update or just current  */
/* 1 rec updt allows multi-field updts & roll-back, multirec does not*/
if (uargs.cmv == 0)
  {
    /* qualify record if op3/op4 &/or op5/op6 spcfd (may be AND/OR)       */
    ss = search34(recb,&uargs);         /* search for op3/4 &/or op5/6    */
    if (ss > 0)
      { /* move constant data into record                                 */
        sncopy(recb+uargs.a1d,uargs.a2c,uargs.a1l,0x00);

        /*Dec07/10 - allow update constant lth < op1 lth & blank excess  */
        /* if op1 lth > op2 lth - clear extra to blanks */
        if (uargs.a1l > uargs.a2l)
          { ll = (uargs.a1l - uargs.a2l);
            memset(recb + uargs.a1d + uargs.a2l,xlc,ll);
          }

        /* rewrite the record & store fileptr for possible roll-back      */
        fileseek("update 1");

        writestat = write(fd1,recb,rsz1);
        /*Sep16/17 - did not reshow updated STL recs */
        vrtyp2 = '\0';  /*Sep16/17 try clear read unused space here */

        if (writestat < 0)
            errmsg("updt write err",fn1,"",0,0x40);
        fplastup = fileptr;             /* save fileptr of last rec updtd */

        /* show update command - if uu vs original (already on screen)    */
        sprintf(opmsg1,"above record updated - %s \n",uargs.cmd);
      }
  }

/*eject*/
else
 {/* multi-record update, from cur fileptr for count spcfd or til EOF*/
  /* u99 0(3),'ABC'                                                  */

  /*Feb26/19 - if u99, reset fileptr=0 (parse already set count 999999999) */
  if (uargs.cmv == 999999999)
    { fileptr = 0;
      fileseek("update u99");
    }

  while(1)
   { rsz1 = getrec(0);
     if (rsz1 <= 0)
      { sprintf(opmsg1,"EOF, %d records read, %d updated %s\n",nn,uu,uargs.cmd);
         /* if any records updated, set fileptr to last rec updated   */
         if (uu)
           { fileptr = fpsv;
             recnum1 = rnsv; /*Jun03/10 also set recnum1 last updated   */
             upnext = 0;     /* inhibit getrec to display correct record*/
             sprintf(opmsg2,"searched to EOF & reset to last record updated\n");
           }
         else
           { ; 
           }
         break;
       }
     nn++;                              /* count recs read for max test*/

     /* qualify record if op3/op4 &/or op5/op6 spcfd (may be AND/OR)       */
     ss = search34(recb,&uargs);         /* search for op3/4 &/or op5/6    */

     if (ss < 0)
       { /* fileptr += rsz1;  May25/10 replace w getrec(0) */
         continue;
       }

     /* update record - move constant data into record                 */
     sncopy(recb+uargs.a1d,uargs.a2c,uargs.a1l,0x00);

     /*Dec07/10 - allow update constant lth < op1 lth & blank excess  */
     /* if op1 lth > op2 lth - clear extra to blanks */
     if (uargs.a1l > uargs.a2l)
       { ll = (uargs.a1l - uargs.a2l);
         memset(recb + uargs.a1d + uargs.a2l,xlc,ll);
       }

/*eject*/
     /* seek to current record & rewrite                               */
     /* May2001: fseek changed to call fileseek (largefiles lseek/lseek64) */
     fileseek("update multiple");

     writestat = write(fd1,recb,rsz1); /*May2001 fwrite -> write for LFS */
     if (writestat < 0)
       { errmsg("updt write err",fn1,"",0,0x40);
       }

     uu++;                         /* count records updated           */
     fpsv = fileptr;               /* save fileptr of last rec updated*/
     rnsv = recnum1;               /* save recnum1 of last rec updated*/

     /* test for max record search limit reached */
     if (nn < uargs.cmv)
       { /* fileptr += rsz1;  May25/10 replace w getrec(0) */
         continue;
       }
     else
       { sprintf(opmsg1,"%d records read, %d updated %s\n",nn,uu,uargs.cmd);
         fileptr = fpsv;    /* so last updated rec will be displayed   */
         recnum1 = rnsv;    /*Jun03/10 also set recnum1 last updated   */
         upnext = 0;        /* inhibit getrec to display correct record*/
         break;
       }
   }
 }
return(1);
}

/*eject*/
/*uvhd------------------------- move1 ----------------------------------*/
/* move field - return 1 if ok, 0 if any error                          */
/*            - examples based on dat1/custmas1 demo file               */
/*  m    35(30),90     - move tel# & contact name following cust name   */
/*  m    35(30),90(12) - move tel# only & clear remainder of op1 lth    */
/*  m    50(20),'DEC 06/2010' - store constant, clear remaining op1 lth */
/*  +1                 - goto next record (or anywhere)                 */
/*  mm                 - repeat prior move (on current record)          */
/*  m99 35(30),90      - move tel# & contact for all recs until EOF     */

int move1(void)
{
int uu;         /* records updated(move) counter    */
int nn;         /* records read searching for moves */
int ss;         /* op3/4 & op4/5 qualify status     */
int ll;         /* calc lth tl clear when op1 lth > op2 lth */
char xlc;       /* eXtra Lth Clear char - blank or null if hex constant */
upnext = 0;     /* inhibit advance to next record on 1st getrec */
uu=0; nn=0;     /* clear counters */
fpsv=0; rnsv=0; 

/* verify option u to allow move */
if ((opsc['u'-'a']) == 0)
  { printf(">>>MOVE requires option u on uvhd command line<<<\n");
    showhelp(help02,26,0x01);
    printf(">>>MOVE requires option u on uvhd command line<<<\n");
    exit(9);
  }
/* save record for possible roll-back of last move to current record */
memcpy(recsav2,recb,rsz1);        /* save record for possible roll-back*/

/* save record for possible roll-back of multi moves to current record */
/* IE - save only if cur fileptr not = fileptr of last move            */
if (fileptr != fplastup)
  { memcpy(recsav1,recb,rsz1);   /* save rec for multi move roll-back  */
  }
/* test for new move or repeat previous move                         */
if (cms[1] == 'm')                      /* repeat prior move ?       */
  { /* trsfr any multi-rec count from mm99 to stored margs structure */
    /* - to allow user to test move on 1 record, then repeat on all  */
    margs.cmv = cargs.cmv;              /* move multi-rec count      */
  }
else
  { /* store new search args - but 1st verify (cargs here, margs below)*/
    if ((cargs.a1l <= 0) || (cargs.a2l <= 0))
      { errmsg("move args invalid",cargs.cmd,"",0,0x10);
        return(0);
      }
    margs = cargs;                      /* store new search args      */
  }

/*May20/13 - set extra lth clear char null, if hex constant, else AEB */
if (margs.a2cc[1] == 'x')
  { xlc = '\0'; }
else
  { xlc = AEB; }

/*eject*/
/* verify move args - op1 lth & op2 field/constant lth > 0 ? */
if ((margs.a1l <= 0) || (margs.a2l <= 0))
  { errmsg("move args invalid",margs.cmd,"",0,0x10);
    return(0);
  }
/*Dec07/10 - verify move op1 dsp+lth not > recsize */
if ((margs.a1d + margs.a1l) > rsz1)
  { errmsg("move op1 dsplcmnt+length > recsize",margs.cmd,"",0,0x10);
    return(0);
  }

/* instruction verified - test for multi-rec move or just current     */
/* 1 rec move allows multi-field moves & roll-back, multirec does not */
if (margs.cmv == 0)
  {
    /* qualify record if op3/op4 &/or op5/op6 spcfd (may be AND/OR)   */
    ss = search34(recd,&margs);         /* search for op3/4 &/or op5/6*/
    if (ss > 0)
      { /* move field or constant data in record */
        /*Dec07/10 - if optn a, constants s/b EBCDIC & clear blank EBCDIC*/
        if (margs.a2cc[0])
          { sncopy(recd+margs.a1d,margs.a2c,margs.a1l,0x00);
          }
        else
          { /* allow overlapping by move to wrecb for move back to recb */
            memcpy(wrecb,recb,rsz1);
            sncopy(recd+margs.a1d,wrecd+margs.a2d,margs.a1l,0x00);
          }

        /*Dec07/10 - allow update constant lth < op1 lth & blank excess  */
        /* if op1 lth > op2 lth - clear extra to blanks */
        if (margs.a1l > margs.a2l)
          { ll = (margs.a1l - margs.a2l);
            memset(recd + margs.a1d + margs.a2l,xlc,ll);
          }

        /* rewrite the record & store fileptr for possible roll-back  */
        fileseek("move 1");

        writestat = write(fd1,recb,rsz1);
        if (writestat < 0)
            errmsg("move write err",fn1,"",0,0x40);
        fplastup = fileptr;             /* save fileptr of last rec moved*/

        /* show move command - if mm vs original (already on screen)     */
        sprintf(opmsg1,"above record move - %s \n",margs.cmd);
      }
  }

/*eject*/
else
 {/* multi-record move, from cur fileptr for count spcfd or til EOF*/
  /* m99 0(3),'ABC'                                                */

  /*Feb26/19 - if m99, reset fileptr=0 (parse already set count 999999999) */
  if (margs.cmv == 999999999)
    { fileptr = 0;
      fileseek("move m99");
    }

  while(1)
   { rsz1 = getrec(0);
     if (rsz1 <= 0)
      { sprintf(opmsg1,"EOF, %d records read, %d move %s\n",nn,uu,margs.cmd);
         /* if any records moved, set fileptr to last rec moved  */
         if (uu)
           { fileptr = fpsv;
             recnum1 = rnsv; /*Jun03/10 also set recnum1 last move   */
             upnext = 0;     /* inhibit getrec to display correct record*/
             sprintf(opmsg2,"searched to EOF & reset to last record moved\n");
           }
         else
           { ; 
           } 
         break;
       }
     nn++;                              /* count recs read for max test*/

     /* qualify record if op3/op4 &/or op5/op6 spcfd (may be AND/OR)       */
     ss = search34(recd,&margs);         /* search for op3/4 &/or op5/6    */

     if (ss < 0)
       { /* fileptr += rsz1;  May25/10 replace w getrec(0) */
         continue;
       }

     /* move field - move field or constant data in record */
     if (margs.a2cc[0])
       { sncopy(recd+margs.a1d,margs.a2c,margs.a1l,0x00);
       }
     else
       { /* allow overlapping by move to wrecb for move back to recb */
         memcpy(wrecb,recb,rsz1);
         sncopy(recd+margs.a1d,wrecd+margs.a2d,margs.a1l,0x00);
       }

     /* if op1 lth > op2 lth - clear extra to blanks */
     if (margs.a1l > margs.a2l)
       { ll = (margs.a1l - margs.a2l);
         memset(recd + margs.a1d + margs.a2l,xlc,ll);
       }

/*eject*/
     /* seek to current record & rewrite */
     /* May2001: fseek changed to call fileseek (largefiles lseek/lseek64)*/
     fileseek("move multiple");

     writestat = write(fd1,recb,rsz1); /*May2001 fwrite -> write for LFS */
     if (writestat < 0)
         errmsg("move/update write err",fn1,"",0,0x40);

     uu++;                         /* count records moved           */
     fpsv = fileptr;               /* save fileptr of last rec moved*/
     rnsv = recnum1;               /* save recnum1 of last rec moved*/

     /* test for max record search limit reached */
     if (nn < margs.cmv)
       { /* fileptr += rsz1;  May25/10 replace w getrec(0) */
         continue;
       }
     else
       { sprintf(opmsg1,"%d records read, %d moved %s\n",nn,uu,margs.cmd);
         fileptr = fpsv;    /* so last moved rec will be displayed     */
         recnum1 = rnsv;    /*Jun03/10 also set recnum1 last moved     */
         upnext = 0;        /* inhibit getrec to display correct record*/
         break;
       }
   }
 }
return(1);
}

/*eject*/
/*uvhd------------------------- moven ----------------------------------*/
/*Dec26/10 - move numeric field - return 1 if ok, 0 if any error        */ 
/*            - examples based on dat1/custmas1 demo file               */
/*  n    180(5p),120(5p)  - move this yr Jan sales to last yr           */
/*  n99  180(5p),120(5p)  - repeat move on all recs until EOF           */
/*  nx12 180(5p),120(5p)  - might add multi-field move in future        */
/*  n    245(9z),120(5p)  - move 5 bytes packed to 9 bytes zoned        */
/*  nn                    - repeat prior move (on current record)       */
/*  n    53(9z),38(6z)    - move 6 bytes to 9 bytes & zero fill extra   */
/*  n    53(9ze),38(6ze)  - move for EBCDIC file                        */

int moven(void)
{
int uu;         /* records updated(move) counter    */
int nn;         /* records read searching for moves */
int ss;         /* op3/4 & op4/5 qualify status     */
int xx;         /* multiple contiguous moven's      */
int nargsa1d;   /* save destination dsplcmnt for multi-record movens */
UVi64 w64;      /* 64 bit work area to retrieve field to move */

upnext = 0;     /* inhibit advance to next record on 1st getrec */
uu=0; nn=0;     /* clear counters */
fpsv=0; rnsv=0; 

/* verify option u to allow move */
if ((opsc['u'-'a']) == 0)
  { printf(">>>move Numeric requires option u on uvhd command line<<<\n");
    showhelp(help02,26,0x01);
    printf(">>>move Numeric requires option u on uvhd command line<<<\n");
    exit(9);
  }

/* save record for possible roll-back of last move to current record */
memcpy(recsav2,recb,rsz1);        /* save record for possible roll-back*/

/* save record for possible roll-back of multi moves to current record */
/* IE - save only if cur fileptr not = fileptr of last move            */
if (fileptr != fplastup)
    memcpy(recsav1,recb,rsz1);   /* save rec for multi move roll-back  */


/* test for new move or repeat previous move                         */
if (cms[1] == 'n')                      /* repeat prior move ?       */
  { /* trsfr any multi-rec count from nn99 to stored nargs structure */
    /* - to allow user to test move on 1 record, then repeat on all  */
    nargs.cmv = cargs.cmv;              /* move multi-rec count      */
  }
else
  { /* store new search args - but 1st verify (cargs here, nargs below)*/
    /*Dec26/10 - allow op2 lth 0 for value stored in dsplcmnt */
    if (cargs.a1l <= 0)
      { errmsg("move numeric args invalid",cargs.cmd,"",0,0x10);
        return(0);
      }
    nargs = cargs;                      /* store new search args      */
  }

/*eject*/
/* verify move args - op1 lth & op2 field/constant lth > 0 ? */
if (nargs.a1l <= 0)
  { errmsg("move numeric args invalid",nargs.cmd,"",0,0x10);
    return(0);
  }
/*Dec07/10 - verify move op1 dsp+lth not > recsize */
if ((nargs.a1d + nargs.a1l) > rsz1)
  { errmsg("move numeric op1 dsplcmnt+length > recsize",nargs.cmd,"",0,0x10);
    return(0);
  }

/* instruction verified - test for multi-rec move or just current     */
/* 1 rec move allows multi-field moves & roll-back, multirec does not */
if (nargs.cmv == 0)
  { /* qualify record if op3/op4 &/or op5/op6 spcfd (may be AND/OR)   */
    ss = search34(recd,&nargs);         /* search for op3/4 &/or op5/6*/
    if (ss > 0)
      { /* move field in 1 record */

        /* allow overlapping by move to wrecb for move back to recb */
        memcpy(wrecb,recb,rsz1);
        w64 = getop(wrecd,&nargs,2);
        /*Nov09/16 - allow option 'x' multiple contiguous moven's */
        xx = 1;                      /* init for 1 move */
        while(1)
          { putop1(recd,&nargs,w64); /* move to 1st/only destination */
            xx++;
            if (xx > nargs.opi['x'-'a'])
              { break;
              }
            nargs.a1d += nargs.a1l;  /* up dsplcmnt to next consec dest*/
          }

        /* rewrite the record & store fileptr for possible roll-back  */
        fileseek("move numeric");

        writestat = write(fd1,recb,rsz1);
        if (writestat < 0)
            errmsg("move numeric write err",fn1,"",0,0x40);
        fplastup = fileptr;             /* save fileptr of last rec moved*/

        /* show move command - if nn vs original (already on screen)     */
        sprintf(opmsg1,"above record move numeric - %s \n",nargs.cmd);
      }
  }

/*eject*/
else
 {/* multi-record move, from cur fileptr for count spcfd or til EOF*/
  nargsa1d = nargs.a1d;     /* save 1st destination dsplcmnt */

  /*Feb26/19 - if n99, reset fileptr=0 (parse already set count 999999999) */
  if (nargs.cmv == 999999999)
    { fileptr = 0;
      fileseek("move numeric n99");
    }

  while(1)
   { rsz1 = getrec(0);
     if (rsz1 <= 0)
      { sprintf(opmsg1,"EOF, %d records read, %d move numeric %s\n"
                      ,nn,uu,nargs.cmd);
         /* if any records moved, set fileptr to last rec moved  */
         if (uu)
           { fileptr = fpsv;
             recnum1 = rnsv; /*Jun03/10 also set recnum1 last move   */
             upnext = 0;     /* inhibit getrec to display correct record*/
             sprintf(opmsg2,"searched to EOF & reset to last record moved\n");
           }
         else
           { ;
           }   
         break;
       }
     nn++;                              /* count recs read for max test*/

     /* qualify record if op3/op4 &/or op5/op6 spcfd (may be AND/OR)       */
     ss = search34(recd,&nargs);         /* search for op3/4 &/or op5/6    */

     if (ss < 0)
       { continue;
       }

     /* allow overlapping by move to wrecb for move back to recb */
     memcpy(wrecb,recb,rsz1);
     w64 = getop(wrecd,&nargs,2);
     /*Nov09/16 - allow option 'x' multiple contiguous moven's */
     xx = 1;                      /* init for 1 move */
     nargs.a1d = nargsa1d;        /* init 1st dest dsplcmnt this record */
     while(1)
       { putop1(recd,&nargs,w64); /* move to 1st/only destination */
         xx++;
         if (xx > nargs.opi['x'-'a'])
           { break;
           }
         nargs.a1d += nargs.a1l;  /* up dsplcmnt to next consec dest*/
       }

/*eject*/
     /* seek to current record & rewrite */
     /* May2001: fseek changed to call fileseek (largefiles lseek/lseek64)*/
     fileseek("move numeric multiple");

     writestat = write(fd1,recb,rsz1); /*May2001 fwrite -> write for LFS */
     if (writestat < 0)
         errmsg("move numeric/update write err",fn1,"",0,0x40);

     uu++;                         /* count records moved           */
     fpsv = fileptr;               /* save fileptr of last rec moved*/
     rnsv = recnum1;               /* save recnum1 of last rec moved*/

     /* test for max record search limit reached */
     if (nn < nargs.cmv)
       { /* fileptr += rsz1;  May25/10 replace w getrec(0) */
         continue;
       }
     else
       { sprintf(opmsg1,"%d records read, %d moved %s\n"
                       ,nn,uu,nargs.cmd);
         fileptr = fpsv;    /* so last moved rec will be displayed     */
         recnum1 = rnsv;    /*Jun03/10 also set recnum1 last moved     */
         upnext = 0;        /* inhibit getrec to display correct record*/
         break;
       }
   }
 }
return(1);
}

/*eject*/
/*uvhd--------------------------- scanrep -------------------------------*/
/* command 'v' for variable length scan/replace                          */
/* scan an area for a pattern & replace with a 2nd pattern               */
/*                                                                       */
/* v 35(25),'ROAD','RD.'      - replace ROAD with RD. anywhere in 35-59  */
/* vv                         - repeat prior scan/rep (on current record)*/
/* v99 35(25),'STREET','ST.'  - repeat until EOF (99 short for 999999999)*/
/* v99 35(25),'AVE.','AVENUE',77(2),'BC' - replace only for BC           */
/* v99w 10(25),'x','x'        - option 'w' write sep file just cust names*/
/*                              tmp/custmas1_yymmdd_HHMMSSV              */
/*Dec08/10 - Write option removed from scanrep (use move + write optn d) */

int scanrep(void)
{
int ss;             /* scanrep return reps in current rec*/
int rr;             /* total replacements made           */
int uu;             /* records updated counter           */
int nn;             /* records read searching for updts  */
int oo;             /* op5/op6 search status             */
int ii,jj,ll,ee;

upnext = 0;         /* inhibit advance to next record on 1st getrec */
rr=0; uu=0; nn=0; fpsv=0; rnsv=0; /* clear counters      */

/* save record for possible roll-back of last update to current record */
memcpy(recsav2,recb,rsz1);        /* save record for possible roll-back*/

/* save record for possible roll-back of multi updts to current record */
/* IE - save only if cur fileptr not = fileptr of last updt            */
if (fileptr != fplastup)
    memcpy(recsav1,recb,rsz1);   /* save rec for multi updt roll-back  */


/* test for new scan/replace or repeat previous scan/replace           */
if (cms[1] == 'v')                      /* repeat prior scan/replace ? */
  { /* trsfr any multi-rec count from vv99 to stored vargs structure   */
    /* - to allow user to test scanrep on 1 record, then repeat on all */
    vargs.cmv = cargs.cmv;              /* update multi-rec count      */
  }
else
  { /* store new search args - but 1st verify (cargs here, vargs below)*/
    if ((cargs.a1l <= 0) || (cargs.a2l <= 0))
      { errmsg("scan/replace args invalid",cargs.cmd,"",0,0x10);
        return(0);
      }
    vargs = cargs;                      /* store new search args      */
  }

/*eject*/
/* verify option u to allow scan/replace */
/* - not required if option 'w' (write sep file) & count spcfd */
if ((vargs.opc['w'-'a']) && (vargs.cmv))
  { ; }
else if (!(opsc['u'-'a']))
  { printf(">>>scan/replace requires option u on uvhd command line<<<\n");
    showhelp(help02,26,0x01);
    printf(">>>scan/replace requires option u on uvhd command line<<<\n");
    exit(9);
  }

/* verify scan/replace args - op1 lth & op2 constant lth > 0 ?        */
if ((vargs.a1l <= 0) || (vargs.a2l <= 0))
  { errmsg("scan/replace args invalid",vargs.cmd,"",0,0x10);
    return(0);
  }

/*Jun04/10 - test option to transfer scanrep args to search args */
/*         - for subsequent 'ss' to verify scanreps              */
if (vargs.opc['s'-'a'])
  { sargs = vargs;               /* transfer scanrep args to search args */
    strcpy(sargs.a2c,sargs.a4c); /* transfer op4 replace to op2 search   */
    sargs.a2l = sargs.a4l;       /* transfer op4 rep lth to search lth   */
    sargs.a4l = 0;               /* disable search on op4,5,6            */
    sargs.a5l = 0;   
    sargs.a6l = 0;
  }

/* instruction verified - test for multi-rec scan/replace or just current  */
/* 1 rec updt allows multi-field updts & roll-back, multirec does not*/
if (vargs.cmv == 0)
 {
  /* call subfunction to perform scan/replace on op1 record field      */
  ss = scanrep1(recb,&vargs);

  /* rewrite updated record - but only if at least 1 replacement made  */
  if (ss)
    { fileseek("scan/replace 1");

      writestat = write(fd1,recb,rsz1); /*May2001 fwrite -> write for LFS */
      if (writestat < 0)
          errmsg("updt write err",fn1,"",0,0x40);
      fplastup = fileptr;               /* save fileptr of last rec updtd */

      /* show scan/replace command - if vv vs original (already on screen)*/
      sprintf(opmsg1,"above record updated - %s \n",vargs.cmd);
    }
 }

/*eject*/
else
 {/* multi-record scan/replace, from cur fileptr for count spcfd or til EOF*/
  /* v99 35(25),'ROAD','RD.'      - replace ROAD with RD. anywhere in 35-59*/
  /*Feb21/19 - record count 99 has already set count high in parse         */
  /* - now if count 999999999 - also reset fileptr to BOF                  */
  if (vargs.cmv == 999999999)
    { fileptr = 0;
      upnext = 0;
      fileseek("scan/replace 99");
    }
  
  while(1)
   { rsz1 = getrec(0);
     if (rsz1 <= 0)
       { sprintf(opmsg1,"EOF, %d read, %d reps in %d records, %s\n"
                      ,nn,rr,uu,vargs.cmd);
         /* if any records updated, set fileptr to last rec updated   */
         if (uu)
           { fileptr = fpsv;
             recnum1 = rnsv;
             upnext = 0;     /* inhibit getrec to display correct record*/
             sprintf(opmsg2,"searched to EOF & reset to last record updated\n");
           }
         else
           { ; 
           } 
         break;
       }
     nn++;                              /* count recs read for max test*/

     /* qualify record - if op5/op6 patterns specified                 */
     if (vargs.a6l)                     /* if op5/op6 pattern length   */
       { oo = search4(recb,&vargs);
         if (oo < 0)
           { /* fileptr += rsz1;  May25/10 replace w getrec(0) */
             continue;
           }
       }

     /* call subfunction to perform scan/replace on op1 record field      */
     ss = scanrep1(recb,&vargs);
     /* scanrep1 return status ss is replacements made in current record  */
     /* if any replacements made - seek to current record & rewrite       */
     if (ss)
       { fileseek("scan/replace multiple"); /* seek to current record  */
         writestat = write(fd1,recb,rsz1);  /* re-write current record */
         if (writestat < 0)
             errmsg("updt write err",fn1,"",0,0x40);
         fpsv = fileptr;          /* save fileptr last rec updated*/
         rnsv = recnum1;          /* save recnum1 last rec updated*/
         uu++;                    /* count records updated        */
         rr +=ss;                 /* count total replacements     */
       }

/*eject*/
     /* test for max record search limit reached                       */
     if (nn < vargs.cmv)
       { /* fileptr += rsz1;  May25/10 replace w getrec(0) */
         continue;
       }
     else
       { sprintf(opmsg1,"%d read, %d reps in %d records, %s\n"
                      ,nn,rr,uu,vargs.cmd);
         fileptr = fpsv;  /* so last updated rec will be displayed   */
         recnum1 = rnsv;  /*Jun03/10 also set recnum1 last updated   */
         upnext = 0;      /* inhibit getrec to display correct record*/
         break;
       }
   }
 }
return(1);
}

/*eject*/
/*uvhd--------------------------- scanrep1 ------------------------------*/
/* scan an area for a pattern & replace with a 2nd pattern               */
/* - called twice by command 'v' for variable length scan/replace        */
/*                                                                       */
/* v 35(25),'ROAD','RD.'      <-- replace ROAD with RD. anywhere in 35-59*/
/*                                                                       */

int scanrep1(char *recb, struct CA *cap)
{
int ii,jj,ee,ss;

/*note - replacing unequal lengths will truncate or blank fill as reqd   */
memset(wa1,' ',cap->a1l);   /* clear work area to copy to                */
ii = cap->a1d;              /* init input index to begin field dsplcmnt  */
ee = cap->a1d + cap->a1l;   /* calc end field index                      */
jj = 0;                     /* init index for work area copy             */
ss = 0;                     /* init count of scanreps this rec (returned)*/

/* begin loop to copy record field to blank filled work area             */
/* - comparing from each byte for match to search pattern on its length  */
/* - replacing on matches & up I/O indices by respective lengths         */
/* - else copy byte by byte until end of record field reached            */
while(ii < ee)
  { if (memcmp(recb+ii,cap->a2c,cap->a2l) == 0)
      { memcpy(wa1+jj,cap->a4c,cap->a4l); /* copy replacement to w/a        */
        jj += cap->a4l;                   /* up output index by replace lth */
        ii += cap->a2l;                   /* up input index by search patlth*/
        ss++;                             /* count reps made this record    */
      }
    else
      { wa1[jj++] = recb[ii++];          /* copy current byte & up indices */
      }
  }

/* now copy work area back to record field */
memcpy(recb+cap->a1d,wa1,cap->a1l);

return(ss);                   /* return scanreps made in this record */
}

/*eject*/
/*uvhd----------------- roll-back last update ------------------------*/
/* via oprtr cmd 'x'                                                  */

int back1(void)
{

/* verify current fileptr = fileptr of last updt                      */
if (fileptr != fplastup)
  { printf("rollback rejected - cur rec not same as last updt\n");
    return(0);
  }

/* May2001: fseek changed to call fileseek (largefiles lseek/lseek64) */
fileseek("rollback x");

writestat = write(fd1,recsav2,rsz1);   /* May2001 fwrite -> write for LFS*/
if (writestat <= 0)
    errmsg("rollback write err",fn1,"",0,0x40);

/* copy cur rec before rollback to rollback save area               */
/* - to allow toggling between updated & not updated                */
memcpy(recsav2,recb,rsz1);             /* update the save area      */

upnext = 0; /* redisplay rolled-back record (inhibit advance to next)*/

return(1);
}

/*eject*/
/*uvhd---------------- roll-back all updts to cur rec ----------------*/
/* via oprtr cmd 'X'                                                  */

int backall(void)
{

/* verify current fileptr = fileptr of last updt                      */
if (fileptr != fplastup)
  { printf("rollback rejected - cur rec not same as last updt\n");
    return(0);
  }

/* May2001: fseek changed to call fileseek (largefiles lseek/lseek64) */
fileseek("rollback X");

writestat = write(fd1,recsav1,rsz1);    /*May2001 fwrite ->write LFS */
if (writestat <= 0)
    errmsg("rollback write err",fn1,"",0,0x40);

/* copy cur rec before rollback to rollback save area               */
/* - to allow toggling between updated & not updated                */
memcpy(recsav1,recb,rsz1);             /* update the save area      */

upnext = 0; /* redisplay rolled-back record (inhibit advance to next)*/

return(1);
}

/*eject*/
/*uvhd-------------------------- drop1 --------------------------------*/
/* drop records via specified pattern &/or by D-ISAM delete flag       */
/*                                                                     */
/* d  8(1),'D'               - drop records with 'D' in column 9       */
/*                                                                     */
/*Apr05/11 - 'drop' command new version - copies 3 files to tmp/...    */
/*                                                                     */
/* $UVTMPDIR/fname_yyyymmdd_hhmmssB - Backup file (all records)        */
/* $UVTMPDIR/fname_yyyymmdd_hhmmssC - Corrected file (non-deleteds)    */
/* $UVTMPDIR/fname_yyyymmdd_hhmmssD - Dropped records                  */
/*                                                                     */
/* After copying out 3 files & closing all including input file:       */
/* - copy the Corrected file back to the original input file           */
/* - reopen & prompt for next command                                  */

/*Feb22/13 - add options a/b c/d to delete by record# ranges           */
/* - reset rec#1 before drop executed, remove option i DISAM deleted   */
/*Jul02/14 - option p control action when both range & pattern apcfd */
/* - default p0 - drop only if pattern match within range            */
/*           p1 - drop either wwithin range or by matching pattern   */

int drop1(void)
{
int aa,bb,cc,dd,dd1,dd2,dd3,ii,nn,mm,ss;

/*Oct21/13 - drop requires option u */
if ((opsc['u'-'a']) == 0)
  { printf(">>>drop records requires option u on uvhd command line<<<\n");
    showhelp(help02,26,0x01);
    printf(">>>drop records requires option u on uvhd command line<<<\n");
    exit(9);
  }

/*Feb22/13 - reset rec#1 before drop executed */
fileptr = 0;         /* reset to begin file */
recnum1 = 0;         /*Apr19/16 - why init 0 in drop, 1 elsewhere ? */
upnext = 0;          /* inhibit advance to next record on 1st getrec */

/* store new drop args                                           */
dargs = cargs;                      /* store new drop args       */

/* open drop output filename                                         */
/* - will overwrite if drop repeated (no collection like write)      */
/*Feb24/11 - add 2nd file for Dropped records as well as Correct recs*/
/*Apr05/11 - add 3rd file for Backup before copy Corrected to input  */
openfileX(bfname,"B",&bfptr,&bfopn,0x00);
openfileX(cfname,"C",&cfptr,&cfopn,0x00);
openfileX(dfname,"D",&dfptr,&dfopn,0x00);

/*eject*/
/* copy to output file from rec#1 */
/*Feb22/13 - drop recs by rec# ranges spcfd by options a/b c/d */
/* drop recs via specified pattern (if any) */
aa=0; bb=0; cc=0; dd=0; ii=0;    /* clear record counters      */
while (1)
 { rsz1 = getrec(0);
   if (rsz1 <= 0)                /* EOF ? */
     { fileptr = filesiz0;
       break;
     }

   aa++;        /* count records in file */
   dd1=0,dd2=0,dd3=0;  /* reset drop flags for current record */

   /*Apr05/11 - copy All records to Backup file (before copy C back)*/
   ss = fwrite(recb,1,rsz1,bfptr);  /* copy to tmp/backup file      */
   bb++;                            /* count records in backup file */
   if (ss <= 0)
      errmsg("writing to Backup file",dargs.cmd,"",0,0x40);

   /*Feb22/13 - drop recs by rec# ranges spcfd by options a/b c/d */
   if ((dargs.opi['a'-'a']) && (dargs.opi['b'-'a']))
     { if ((aa >= dargs.opi['a'-'a']) && (aa <= dargs.opi['b'-'a']))
         { dd1++;      /* set drop by range for test below */
         }
     }
   if ((dargs.opi['c'-'a']) && (dargs.opi['d'-'a']))
     { if ((aa >= dargs.opi['c'-'a']) && (aa <= dargs.opi['d'-'a']))
         { dd1++;      /* set drop by range for test below */
         }
     }

   /* if pattern spcfd - bypass matching records */
   if (dargs.a2l)
     { mm = search234(recb,&dargs);   /* 9701 search = > < ! */
       if (mm >= 0)
         { dd2++;      /* set drop by pattern for test below */
         }
     }

/*eject*/
    /*Jul02/14 - option p control action when both range & pattern apcfd */
    /* - default p0 - drop only if pattern match within range            */
    /*           p1 - drop either wwithin range or by matching pattern   */
   if (dargs.opi['p'-'a'] & 0x01)
     { dd3 = (dd1 + dd2);
     }
   else
     { if (dd1 && dd2)
         { dd3++;
         }
     }

   /*Feb22/13 - test drop flag set by rec# ranges or by drop pattern */
   if (dd3)
     { dd++;                      /* count records dropped by pattern*/
       ss = fwrite(recb,1,rsz1,dfptr);  /* copy to tmp/dropped file  */
       if (ss <= 0)
          errmsg("writing to Dropped file",dargs.cmd,"",0,0x40);
       continue;                  /* repeat loop to get next rec     */
     }

   /* No delete flags - copy current record to tmp/drop file            */
   ss = fwrite(recb,1,rsz1,cfptr);    /* copy to Correct outfile        */
   if (ss <= 0)
      errmsg("copying/dropping to output file",dargs.cmd,"",0,0x40);
   cc++;                              /* count Correct records          */

   /* test max copy count reached, if so display confirmation & return  */
   if ((dargs.cmv) && (cc >= dargs.cmv))
     { break;
     }
 }

/*eject*/
/* copy loop ended by max count, or EOF - store stats msg & close files */
sprintf(opmsg1,"%d Backup, %d Correct(not Dropped), %d Dropped\n",bb,cc,dd);

closefileX(bfname,"B",&bfptr,&bfopn,&bb,0x00);  /* close Backup  outfile */
closefileX(cfname,"C",&cfptr,&cfopn,&cc,0x00);  /* close Correct outfile */
closefileX(dfname,"D",&dfptr,&dfopn,&dd,0x00);  /* close Dropped outfile */

/*Apr05/11 - copy Corrected file back to the original input file  */
/*         - reopen & prompt for next command                     */
close (fd1);                        /* close original input file  */
sprintf(w1,"cp %s %s",cfname,fn1);  /* format cp command          */
system(w1);                         /* execute cp command         */
rcount = cc;                        /* set new file count         */
recnum1 = 0;         /*Apr19/16 - why init 0 in drop, 1 elsewhere ? */
fileptr = 0;
fileopen();                         /* reopen Corrected input file*/
rsz1 = getrec(0);                   /* get 1st record             */
/*Mar05/14 - reset fileptr before re-open & get 1st rec after open*/
/*     - also clear opmsg2 with EOF msg stored at EOF of filecopy */
opmsg2[0] = '\0';

/*May03/11 - force Index rebuild if varlth file */
/* if ((ftype != 'f') && (ftype != 't'))        */
/*Feb22/13 - should I Re-build Index on Text files ?? */
if (ftype != 'f')
  { opsi['f'-'a'] |= 0x02;  /* set bit to force rebuild */
    fii1 = 0;               /* reset Index build switch */
    findex();               /* call function to rebuild */
    vrtyp2 = '\0';          /*Sep15/17 clear read unused space */
    IndexBuild = 0;         /* reset for STL x4y4j4 deletes only */
  }

return(1);
}

/*eject*/
/*uvhd-------------------------- check1 ---------------------------------*/
/* check sequence - display 1st record not in sequence                   */
/*  c  0(6)       - check sequence of cols 1-6                           */

int check1(void)
{
int cc;
int dd,ll;       /* save seqnum field dsplcmnt & length */
int ii;          /* option i value */

upnext = 0;     /* inhibit advance to next record on 1st getrec */

/* test new chkseq cmd or repeat previously stored cmd (cc)             */
if (cms[1] == 'c')
   ;
else
  { /* store new chkseq args - 1st verify args                          */
    if (cargs.a1l <= 0)
      { errmsg("check sequence cmd must specify field dsplcmnt(lth)"
               ,cargs.cmd,"",0,0x10);
        return(0);
      }
    zargs = cargs;                 /* store chkseq args                 */
  }

/* verify chkseq args - op1 lth > 0                                     */
if (zargs.a1l <= 0)
  { errmsg("check sequence cmd must specify field dsplcmnt(lth)"
           ,zargs.cmd,"",0,0x10);
    return(0);
  }

/* init seq chk from current record util EOF (or seq err)              */
/* get current record & save seqchk field for compare to following rec */
rsz1 = getrec(0);                   /* get 1st rec for chk seq loop      */
if (rsz1 <=0)
  { errmsg("check seq at EOF already ?, reset to BOF & retry"
           ,zargs.cmd,"",0,0x10);
    return(0);
  }

dd = zargs.a1d;                    /* save seqnum field dsplcmnt    */
ll = zargs.a1l;                    /* save seqnum field length      */
memcpy(chksav1,recb+dd,ll);        /* save seqnum field from 1st rec*/

/*eject*/
/* begin loop to check sequence - until EOF (or seq err)               */
while (1)
  { /* fileptr += rsz1;  May25/10 replace w getrec(0) */
    rsz1 = getrec(0);                 /* get next record            */
    if (rsz1 <=0)
      { sprintf(opmsg1,"%s --> sequence OK, reached EOF with no errors\n"
               ,zargs.cmd);
        upnext = 0;    /* inhibit advance to next record on 1st getrec */
        return(1);
      }

    memcpy(chksav2,recb+dd,ll);     /* get seqnum from current record  */

    /* test option to check by specified increment - option i1,i10,etc */
    if (zargs.opc['i'-'a'] == 'i')
      { ii = zargs.opi['i'-'a'];    /* save seq# option increment */
        if (ii == 0)
          { ii = 1;                 /* default to 1 if unspcfd    */
          }
        chksav1i = atol2(chksav1,'~',ll,0x01); /* cnvt prior rec# to int*/
        chksav2i = atol2(chksav2,'~',ll,0x01); /* cnvt current rec# to int*/
        chksav1i += ii;                        /* prior val + incrmnt   */
        if (chksav1i != chksav2i)
          { break;
          }
      }
    else
      { /* character compare current record seq field to save area     */
        cc = memcmp(chksav2,chksav1,ll);
        if ((cc < 0) && (zargs.opc['d'-'a'] != 'd'))
            break;
        if ((cc > 0) && (zargs.opc['d'-'a'] == 'd'))
            break;
        if ((cc == 0) && (zargs.opc['e'-'a'] == 'e'))
            break;
      }

    /* current rec OK - save seq fld (for == chk) & repeat loop        */
    memcpy(chksav1,chksav2,ll); /* save current seqnum for next rec chk*/
  }

/* loop broken by out of sequence - display errmsg & return            */
sprintf(opmsg1,"%s ERROR, 1st out of sequence record above\n",zargs.cmd);
upnext = 0;            /* inhibit advance to next record on 1st getrec */
return(1);             /* return (1) to show next record               */
}

/*eject*/
/*uvhd------------------------ enumerate -------------------------------*/
/* enumerate(count) records (from current record to EOF or within max spcfd*/
/* - or records matching a pattern, to a specified maximum              */
/*                                                                      */
/* t                 - count recs from current point to EOF             */
/*                                                                      */
/* t500 0(80),='ABC' - count records with 'ABC' anywhere in 1st 80 bytes*/
/*                     (count from current record for 500 max or EOF)   */
/*                                                                      */
/* tt                        - repeat previous count                    */
/*                                                                      */
/* - if nofind set fileptr to last rec in file & return 0               */

int enum1(void)
{
int nn;           /* max records search ctr       */
int mm;           /* search find/nofind 0+/-      */
int tt;           /* enumerate counter            */

upnext = 0;       /* inhibit advance to next record on 1st getrec */
nn=0; tt=0; fpsv=0; rnsv=0;     /* clear counters */

/* test for 'e=s' transfer last search cmd to enumerate cmd storage structure */
if ((cms[1] == '=') && (cms[2] == 's'))
  { eargs = sargs;              /* store enum cmd from last search cmd  */
    eargs.cmd[0] = 'e';         /* change cmd from 's' to 'e'           */
    eargs.cmc[0] = 'e';         /* change cmc from 's' to 'e'           */
    printf("enumerate args stored from last search: %s \n",eargs.cmd);
    printf("execute 'ee' to enumerate spcfd records from current to EOF --> ");
    return(2);
  }

/* test for new enumerate command or repeat previous enumerate command (ee)*/
if (cms[1] == 'e')                      /* repeat prior enumerate ?        */
  { /* allow value updt on repeat cmd - tt99                               */
    if (cargs.cmv)
        eargs.cmv = cargs.cmv;
  }
else
  { /* store new enumerate args                                          */
    eargs = cargs;                      /* store new enumerate args      */
  }

/*Jun04/10 - test option to transfer enumerate args to search args */
/*         - for subsequent 'ss' to verify enumerates              */
if (eargs.opc['s'-'a'])
  { sargs = eargs;       /* transfer enumerate args to search args */
    sargs.a4l = 0;       /* disable search on op4,5,6              */
    sargs.a5l = 0;   
    sargs.a6l = 0;
  }

/*eject*/
/* enumerate (count) recs matching any spcfd patterns */
/* - within max recs (if spcfd), or until EOF         */

/*Feb26/19 - if e99, reset fileptr=0 (parse already set count 999999999) */
if (eargs.cmv == 999999999)
  { fileptr = 0;
    fileseek("enumerate e99");
  }

while (1)
 { rsz1 = getrec(0);
   if (rsz1 <= 0)                       /* EOF ?      */
     { break;
     }
   nn++;                              /* count records read */

   /* if enumerate pattern spcfd - search & count matching recs to max cnt*/
   if (eargs.a2l)
     { mm = search234(recb,&eargs);   /* search for patterns spcfd    */
       if (mm >= 0)
         { tt++;                      /* count matching records        */
           fpsv = fileptr;            /* save fileptr of last match    */
           rnsv = recnum1;            /* save recnum1 last rec matched */
         }
     }
   else
     { tt++;                          /* count all recs (no pattern spcfd)*/
       fpsv = fileptr;                /* save fileptr of last counted    */
       rnsv = recnum1;                /* save recnum1 last rec matched   */
     }
   /* test max enumerate limit reached, if so break read loop now    */
   if ((eargs.cmv) && (nn >= eargs.cmv))
     { break;
     }
   /* increment fileptr to next record & repeat loop */
   /* fileptr += rsz1;  May25/10 replace w getrec(0) */
 }

/* enumerate loop ended by max count, or EOF                         */
/* - store results msg to be displayed after last rec read           */
sprintf(opmsg1,"%s --> %d recs counted of %d read\n",eargs.cmd,tt,nn);
sprintf(opmsg2,"searched to EOF & reset to last record counted\n");

/* if pattern search found any matches, set fileptr to last match */
if (eargs.a2l && tt)
  { fileptr = fpsv;            /* store fileptr of last match     */
    recnum1 = rnsv;            /* store recnum1 of last match     */
  }

/*May26/10 - force read lastrec/display with opmsg1 in case enum all to EOF*/
upnext = 0;      /* inhibit getrec to display correct record*/

return(1);
}

/*eject*/
/*uvhd------------------------ konvert ------------------------------*/
/*Mar24/14 - Konvert command added                                   */
/*         - converts for display only (no record updates)           */
/* kn  88(4b)       <-- convert 4 bytes binary to numeric display    */
/* kn  92(5p)       <-- convert 5 byes packed to numerc display      */
/*                                                                   */
/* kh4 0(8z)        <-- convert 8 bytes zoned numeric to hex 4 bytes */
/* kh  92(5p)       <-- convert 5 byes packed to hex display (4 dflt)*/
/*                                                                   */
/* kh  '12345678'   <-- convert numeric to hex display               */
/* kn  x'12345678'  <-- convert hex value to numeric                 */
/*                                                                   */
/* kk               <-- repeat previous konvert                      */

int konvert(void)
{
UVi64 w64;        /* op1 retrieved to binary */
char hex1[20];    /* hexchars converted from binary */
char hex2[20];    /* hexchars to be displayed */
int hh;           /* lth of hex byte display (dflt 4) */
int hh2;          /* * 2 for output lth */
int ll;

/* test for to repeat previous konvert command (kk) */
if (cms[1] == 'k')        /* repeat prior konvert ? */
  { ;
  }
else
  { /* store new acum args */
    kargs = cargs;  
  }

/* test for constant, shifted from op1 to op2 by parse() */
if (arg1x)
  { /* allow for hex constant x'----' vs '----' */
    if (kargs.a2dt == 'x')
      { /* store hex constant in w64 left justified */
        w64 = 0;
        memcpy((char*)&w64,kargs.a2c,kargs.a2l);
      }
    else
      { /* store numeric constant rt justified & convert to binary*/
        memset(w2,'0',20);
        w2[20] = '\0';
        ll = (20 - kargs.a2l);
        memcpy(&w2[ll],kargs.a2c,kargs.a2l);
        w64 = strtoll(w2,(char**)0,10);
      }
  }
else
  { /* get op1 binary,packed, or numeric into 64 bit integer */
    w64 = getop(recb,&kargs,1);
  }

/*eject*/
/* display depending onoptions 'h' or 'n' (default) */
if (kargs.opc['h'-'a'] == 'h')
  { /* display hex char representation of binary value */
    /* - preseume 4 byte display if unspcfd */
    hh = kargs.opi['h'-'a']; 
    if (!hh)
      { hh = 4;
      }

    /* convert 8 bytes binary to 16 bytes hex chars */
    data2hex(hex1,(char*)&w64,16,0x02);
    hh2 = (hh * 2);             /* calc display length   */
    sncopy(hex2,hex1,hh2,0x03); /* copy & null terminate */
    printf("%s --> %s\n",kargs.cmd,hex2);
  }
else
  { printf("%s --> %ld\n",kargs.cmd,w64);
  }

return(1);
}

/*eject*/
/*uvhdcob--------------------- accumulate ------------------------------*/
/* accumulate a record field (from current to EOF or within max spcfd)  */
/* - or acum records matching a pattern, to a specified maximum         */
/*                                                                      */
/* a 120(5p)    <-- acum 120-124 5 byte packed from current point to EOF*/
/*                                                                      */
/* a 120(5p),,77(2),'BC'  <-- acum records with 'BC' in 77-78 (til EOF) */
/*                                                                      */
/* aa                     <-- repeat previous acum                      */

int acum1(void)
{
UVi64 acumr;      /* accum field retrvd from current record */
UVi64 acumx;      /* accum crossfoot option x               */
UVi64 acumt;      /* accum total from all records this cmd  */
int aa;           /* records accumulated   */
int nn;           /* max records ctr       */
int mm;           /* search match/nomatch  */
int xx;           /* crossfoot option value*/
int ii;

upnext = 0;       /* inhibit advance to next record on 1st getrec */
nn=0; aa=0; fpsv=0; rnsv=0;     /* clear counters */
acumt = 0;        /* init acum total */

/* test for new acum command or repeat previous acum command (ee)*/
if (cms[1] == 'a')                      /* repeat prior acum ?   */
  { /* allow value updt on repeat cmd - aa99                     */
    if (cargs.cmv)
        aargs.cmv = cargs.cmv;
  }
else
  { /* store new acum args */
    aargs = cargs;  
  }

/*Feb26/19 - if a99, reset fileptr=0 (parse already set count 999999999) */
if (aargs.cmv == 999999999)
  { fileptr = 0;
    fileseek("accumulate a99");
  }

/*eject*/
/* acum recs matching any spcfd patterns       */
/* - within max recs (if spcfd), or until EOF  */
while (1)
 { rsz1 = getrec(0);
   if (rsz1 <= 0)                     /* EOF ? */
     { break;
     }
   nn++;                               /* count records read */

   /* if acum pattern spcfd - search & acum matching recs to max cnt  */
   if (aargs.a3l)
     { mm = search34(recd,&aargs);       /* search for patterns spcfd   */
       if (mm >= 0)
         { aa++;                         /* acum matching records       */
           /*Feb26/11 - add crossfoot option x */
           /* - copy args structure so we can increment a1d by a1l each xft*/
           xargs = aargs;                /* init args for this record   */
           xx = aargs.opi['x'-'a'];      /* retrieve crossfoot value    */
           if (xx <= 0) { xx = 1;}       /* default 1 field             */
           acumx = 0;                    /* init xft acum this record   */
           for (ii=0; ii < xx; ii++)
              { acumr = getop(recd,&xargs,1); /* retrieve current xft fld*/
                acumx += acumr;          /* acum xft total this record   */
                xargs.a1d += xargs.a1l;  /* up dsplcmnt by fld lth       */
              }
           acumt += acumx;               /* add to acum total this cmd  */
           fpsv = fileptr;               /* save fileptr of last match   */
           rnsv = recnum1;               /* save recnum1 last rec matched*/
         }
     }
   else
     { aa++;                           /* acum all recs (no pattern spcfd)*/
       /*Feb26/11 - add crossfoot option x */
       /* - copy args structure so we can increment a1d by a1l each xft*/
       xargs = aargs;                /* init args for this record   */
       xx = aargs.opi['x'-'a'];      /* retrieve crossfoot value    */
       if (xx <= 0) { xx = 1;}       /* default 1 field             */
       acumx = 0;                    /* init xft acum this record   */
       for (ii=0; ii < xx; ii++)
          { acumr = getop(recd,&xargs,1); /* retrieve current xft fld*/
            acumx += acumr;          /* acum xft total this record   */
            xargs.a1d += xargs.a1l;  /* up dsplcmnt by fld lth       */
          }
       acumt += acumx;                 /* add to acum total this cmd      */
       fpsv = fileptr;                 /* save fileptr of last acum       */
       rnsv = recnum1;                 /* save recnum1 last rec matched   */
     }

   /* test max acum limit reached, if so break read loop now */
   if ((aargs.cmv) && (nn >= aargs.cmv))
     { break;
     }

   /* increment fileptr to next record & repeat loop */
   /* fileptr += rsz1;  May25/10 replace w getrec(0) */
 }

/*eject*/
/* acum loop ended by max count, or EOF */
/* - store results msg to be displayed  */
sprintf(opmsg1,"%s --> acum total=%ld from %d recs of %d read\n"
              ,aargs.cmd,acumt,aa,nn);
sprintf(opmsg2,"searched to EOF & reset to last record accumulated\n");

/* if pattern search found any matches, set fileptr to last match */
if (aargs.a3l && aa)
  { fileptr = fpsv;            /* store fileptr of last match     */
    recnum1 = rnsv;            /* store recnum1 of last match     */
  }

/*May26/10 - force read lastrec/display with opmsg1 in case acum all to EOF*/
upnext = 0;      /* inhibit getrec to display correct record*/

return(1);
}

/*eject*/
/*uvhd------------------------- translate --------------------------------*/
/* translate current record - return 1 if ok, 0 if any error              */
/*                   - sample commands follow:                            */
/*  tt               - repeat prior translate (on current record)         */

int trans(void)
{
int tt;               /* records translated counter */
char *pp;

upnext = 0;           /* inhibit advance to next record on 1st getrec */
tt=0; fpsv=0; rnsv=0; /* clear counters */

/* verify command line option u to allow translate                    */
if ((opsc['u'-'a']) == 0)
  { printf(">>>Translate requires option u on uvhd command line<<<\n");
    showhelp(help02,26,0x01);
    printf(">>>Translate requires option u on uvhd command line<<<\n");
    exit(9);
  }

/* save record for possible roll-back of last update to current record */
memcpy(recsav2,recb,rsz1);        /* save record for possible roll-back*/

/* save record for possible roll-back of multi translates to current record */
/* IE - save only if cur fileptr not = fileptr of last translate            */
if (fileptr != fplastup)
    memcpy(recsav1,recb,rsz1);   /* save rec for multi translate roll-back  */


/* test for new translate or repeat previous translate                   */
if (cms[1] == 't')                      /* repeat prior translate ?      */
  { /* trsfr any multi-rec count from tt99 to stored targs structure     */
    /* - to allow user to test translate on 1 record, then repeat on all */
    targs.cmv = cargs.cmv;              /* translate multi-rec count     */
  }
else
  { /* default op1 to entire record if not specified */
    if (cargs.a1l <= 0)
      { cargs.a1l = rsz1;
      }
    targs = cargs;                      /* store new translate args  */
  }

/* verify translate option - present & 1 of a,e,u,l,c,p */
pp = strchr("aeulcps",targs.cmo[0]);
if ((targs.cmo[0]) && (pp))
  { ;
  }
else
  { errmsg("translate option must be 1 of a,e,u,l,c,p",targs.cmd,"",0,0x10);
    return(0);
  }

/*eject*/
/* test for multi-rec translate or just current                      */
/* 1 rec translate allows multi-field translates & roll-back, multi does not*/
if (targs.cmv <= 0)
  {
    tran1();                           /* translate current record   */
    tt++;                              /* count records translated(1)*/

    /* rewrite the record & store fileptr for possible roll-back     */
    fileseek("translate 1");

    writestat = write(fd1,recb,rsz1);
    if (writestat < 0)
        errmsg("translate write err",fn1,"",0,0x40);
    fplastup = fileptr;             /* save fileptr of last rec translated */

    /* show translate command - if tt vs original (already on screen) */
    sprintf(opmsg1,"above record translated --> %s \n",targs.cmd);
  }

/*eject*/
else
 {/* multi-record translate, from cur fileptr for count spcfd or til EOF*/
  /*Feb26/19 - if t99, reset fileptr=0 (parse already set count 999999999) */
  if (targs.cmv == 999999999)
    { fileptr = 0;
      fileseek("translate t99");
    }

  while(1)
   { rsz1 = getrec(0);
     if (rsz1 <= 0)
       { sprintf(opmsg1,"EOF, %d records translated %s\n",tt,targs.cmd);
         /* if any records translated, set fileptr to last rec translated */
         if (tt)
           { fileptr = fpsv;  /* store fileptr of last match */
             recnum1 = rnsv;  /* store recnum1 of last match */
             upnext = 0;      /* inhibit getrec to display correct record*/
          sprintf(opmsg2,"searched to EOF & reset to last record translated\n");
           }
         else
           { ; 
           } 
         break;
       }
     tran1();                           /* translate current record    */
     tt++;                              /* count recs translated for max test*/

     /* seek to current record & rewrite                               */
     fileseek("translate multiple");

     writestat = write(fd1,recb,rsz1); /*May2001 fwrite -> write for LFS */
     if (writestat < 0)
         errmsg("translate write err",fn1,"",0,0x40);

     fpsv = fileptr;          /* save fileptr of last rec translated*/
     rnsv = recnum1;          /* save recnum1 last rec translated   */

     /* test for max record translate limit reached                    */
     if (tt < targs.cmv)
       { /* fileptr += rsz1;  May25/10 replace w getrec(0) */
         continue;
       }
     else
       { sprintf(opmsg1,"%d records translated %s\n",tt,targs.cmd);
         fileptr = fpsv;  /* so last translate rec will be displayed */
         recnum1 = rnsv;  /*Jun03/10 also set recnum1 last updated   */
         upnext = 0;      /* inhibit getrec to display correct record*/
         break;
       }
   }
 }
return(1);
}

/*eject*/
/*uvhd-------------------------- tran1 ----------------------------------*/
/* translate current record - called by single & multiple translates     */

int tran1(void)
{
int dd,ll,ee;      /* op1 dsplcmnt,length,end from targs structure */
int opi;           /* numeric value (bits) from option 'c' or 'p'  */
char opc;          /* translate option a,e,u,l,c,p                 */
char clr;          /* unprintables clear char (blank or period)    */
int ii;

/* store dsplcmnt, length, options,& clear char from targs structure */
dd = targs.a1d;
ll = targs.a1l;
ee = dd + ll;
opc = targs.cmo[0];
if (opc == 'c')
  { opi = targs.opi['c'-'a'];
    clr = ' ';
  }
if (opc == 'p')
  { opi = targs.opi['p'-'a'];
    clr = '.';
  }

/* test translate option a,e,u,l,c,p,s */
if (opc == 'a')
  { toascii2((Uchar*)recb+dd,ll,0x00);
  }
else if (opc == 'e')
  { toebcdic2((Uchar*)recb+dd,ll,0x00);
  }
else if (opc == 'u')
  { toupper2(recb+dd,ll,0x00);
  }
else if (opc == 'l')
  { tolower2(recb+dd,ll,0x00);
  }
/*Nov11/11 - Translate zoned Sign from mainframe to Micro Focus convention */
else if (opc == 's')
  { for (ii = dd; ii < ee; ii++)
     { recb[ii] = EBs2MFs[(unsigned char)(recb[ii])];
     }
  }

/*eject*/
else /* must be c or p */
  { /* copy neutral translate table to work area                      */
    /* & modify unprintables to blanks or periods depending on c or p */
    memcpy(trtc,trtn,256);
    memset(trtc,clr,32);
    memset(trtc+127,clr,129);

    /* restore LF,CR,FF,Tab depending on option bits                */
    if (opi & 0x01)
        trtc[10] = 0x0A;
    if (opi & 0x02)
        trtc[13] = 0x0D;
    if (opi & 0x04)
        trtc[12] = 0x0C;
    if (opi & 0x08)
        trtc[9] = 0x09;

    /* now translate the op1 area */
    for (ii = dd; ii < ee; ii++)
      { recb[ii] = trtc[(unsigned char)(recb[ii])];
      }
  }

return(1);
}

/*eject*/
/*uvhd------------------------- seqgen --------------------------------*/
/* generate sequence number records - return 1 if ok, 0 if any error   */
/*                                                                     */
/*  g99i10 70(6),'#1000_'   <-- example#1                              */
/*  - inserts any non-numeric constants before &/or after seqgen digits*/
/*  - starting sequence# may be specified                              */
/*  - will seq# in contiguos digit locations spcfd                     */
/*  - may specify desired increment with option i (10 above)           */
/*  - must specify max number of records to sequence# (else only 1)    */
/*                                                                     */
/*  g99    70(6),'A00001',0(1),'A'    <-- example #2                   */
/*  - inserts seq# in records with 'A' in 1st byte                     */
/*                                                                     */
/*  gg               - repeat prior seqgen (on current record)         */

int seqgen(void)
{ int uu;                  /* records updated counter               */
int nn;                    /* records read searching for updts      */
int ss;                    /* op3/4 & op4/5 qualify status          */
int dd;                    /* dsplcmnt to 1st digit in op1 constant */
int ll;                    /* length of seqgen 1st digit to nondigit*/
int ii;
int seqgeni;               /* seqgen increment (dflt 1 or optn i)   */
int seqgenv;               /* seqgen ctr value integer              */
char seqgenc[15];          /* seqgen ctr value characters           */
upnext = 0;                 /* inhibit advance to next on 1st getrec */
uu=0; nn=0; fpsv=0; rnsv=0; /* clear counters */

/* verify option u to allow seqgen */
if ((opsc['u'-'a']) == 0)
  { printf(">>>generate seqnum requires option u on uvhd command line<<<\n");
    showhelp(help02,26,0x01);
    printf(">>>generate seqnum requires option u on uvhd command line<<<\n");
    exit(9);
  }

/* test for new seqgen or repeat previous seqgen                      */
if (cms[1] == 'g')                      /* repeat prior seqgen ?      */
  { /* trsfr any multi-rec count from gg99 to stored gargs structure  */
    /* - to allow user to test seqgen on 1 record, then repeat on all */
    gargs.cmv = cargs.cmv;              /* seqgen multi-rec count     */
  }
else
  { /* store new search args - but 1st verify (cargs here, gargs below)*/
    if ((cargs.a1l <= 0) || (cargs.a2l <= 0))
      { errmsg("generate seqnum args invalid",cargs.cmd,"",0,0x10);
        return(0);
      }
    if (cargs.a1l != cargs.a2l)
      { errmsg("seqnum field lth NOT= constant lth",cargs.cmd,"",0,0x10);
        return(0);
      }
    if (cargs.cmv == 0)
      { cargs.cmv = 999999999;     /* default count high if not spcfd */
      }
    gargs = cargs;                 /* store new search args           */
  }
/*eject*/
/* verify seqgen args - op1 lth & op2 constant lth > 0 ?              */
if ((gargs.a1l <= 0) || (gargs.a2l <= 0))
  { errmsg("seqnum args invalid",gargs.cmd,"",0,0x10);
    return(0);
  }

/* verify seqgen args - op1 lth = op2 constant lth ?                 */
if (gargs.a1l != gargs.a2l)
  { errmsg("generate seqnum field lth NOT= constant lth",gargs.cmd,"",0,0x10);
    return(0);
  }

/* store seqgen increment from option 'i' or default to 1          */
if (gargs.opi['i'-'a'])
  { seqgeni = gargs.opi['i'-'a'];  /* store seqgen inc from optn i */
  }
else
  { seqgeni = 1;                   /* default seqgen increment to 1*/
  }

/* extract initial value for seqgen from op1 constant          */
seqgenv = atol2(gargs.a2c,'~',15,0x01);

/* determine seqgen dsplcmnt & length by scanning op1 constant */
/* - for 1st numeric & then 1st non-numeric                    */
for (ii=0; gargs.a2c[ii]; ii++)
  { if (isdigit(gargs.a2c[ii]))
      { break;
      }
  }
dd = ii;       /* save dsplcmnt of 1st digit within op1 constant */

for ( ; gargs.a2c[ii]; ii++)
  { if (!isdigit(gargs.a2c[ii]))
      { break;
      }
  }
ll = ii - dd;  /* save lth of seqgen */

/* verify seqgen lth from 1 to 10 digits */
if ((ll > 10) || (ll < 1))
  { errmsg("sequence number digits min 1 max 10",gargs.cmd,"",0,0x10);
    return(0);
  }

/*eject*/
/* multi-record seqgen, from cur fileptr for count spcfd or til EOF*/
/* u99 0(3),'ABC'                                                  */

/*Feb26/19 - if g99, reset fileptr=0 (parse already set count 999999999) */
if (gargs.cmv == 999999999)
  { fileptr = 0;
    fileseek("generate seq#s g99");
  }

while(1)
  { rsz1 = getrec(0);
    if (rsz1 <= 0)
      { sprintf(opmsg1,"EOF, %d records read, %d seqnumd %s\n",nn,uu,gargs.cmd);
        /* if any records seqgend, set fileptr to last rec seqgend   */
        if (uu)
          { fileptr = fpsv;  /* store fileptr if last match */
            recnum1 = rnsv;  /* store recnum1 of last match */
            upnext = 0;      /* inhibit getrec to display correct record*/
           sprintf(opmsg2,"searched to EOF & reset to last record sequenced\n");
          }
        else
          { ;
          } 
        break;
      }
    nn++;                              /* count recs read for max test*/

    /* qualify record if op3/op4 &/or op5/op6 spcfd (may be AND/OR)       */
    ss = search34(recb,&gargs);         /* search for op3/4 &/or op5/6    */
    if (ss < 0)
      { /* fileptr += rsz1;  May25/10 replace w getrec(0) */
        continue;
      }

    /* move any constant data before &/or after seqgen into record      */
    /* - then move the current value of seqgen ctr into digit positions */
    sncopy(recb+gargs.a1d,gargs.a2c,gargs.a1l,0x00);  /* store contants */
    sprintf(seqgenc,"%010d",seqgenv);          /* convert int to chars  */
    sncopy(recb+gargs.a1d+dd,seqgenc+10-ll,ll,0x00); /* insert seqgen digits*/

/*eject*/
    /* seek to current record & rewrite                               */
    /* May2001: fseek changed to call fileseek (largefiles lseek/lseek64) */
    fileseek("seqgen multiple");
    writestat = write(fd1,recb,rsz1); /*May2001 fwrite -> write for LFS */
    if (writestat < 0)
      { errmsg("updt write err",fn1,"",0,0x40);
      }
    fpsv = fileptr;          /* save fileptr of last rec seqgend */
    rnsv = recnum1;          /* save recnum1 last rec matched    */

    /* increment seqgen, count records update, & test for max search limit*/
    seqgenv += seqgeni;               /* increment by optn i or dflt 1    */
    uu++;                             /* count records seqgend            */

    /* test for max record search limit reached                       */
    if (nn < gargs.cmv)
      { /* fileptr += rsz1;  May25/10 replace w getrec(0) */
        continue;
      }
    else
      { sprintf(opmsg1,"%d records read, %d seqnumd %s\n",nn,uu,gargs.cmd);
        fileptr = fpsv;    /* so last seqgend rec will be displayed   */
        recnum1 = rnsv;    /*Jun03/10 also set recnum1 last updated   */
        upnext = 0;        /* inhibit getrec to display correct record*/
        break;
      }
  }
return(1);
}

/*eject*/
/*uvhd------------------------- trunc1 -----------------------------------*/
/* adjust filesize (truncate or extend with x'00's)                       */
/*                                                                        */
/* z100000   - set filesize to 100,000 bytes                              */
/*                                                                        */
/* - 'z' truncate not handled by uvhd.exe (compiled by lcc/gcc/MinGW)*/
/* - truncate is defined by #include unistd.h on unix/linux (n/a windows) */
/* - bypassed (with errmsg) when compiled on windows by testing DWIN      */

int trunc1(void)
{
int ss;

if (!opsc['u'-'a'])
   errmsg("z (truncate) requires update option u on cmd line"
          ,cargs.cmd,fn1,0,0x21);

close(fd1);                    /*May2001 fclose -> close for LFS     */

/* store new filesize, calc recs & rmndr, display results, continue ?*/
filesize = cmn;                /* store new filesize                  */
chkrcsz(rszo);                 /* check filesize/rcsz evenly divisible*/
printf("filesize=%s recsize=%d, fs/rs=%d, rmndr=%d (s/b 0) OK ? y/n --> "
        ,fsedit,rszo,rcount,rmndr);
fgets(args,80,stdin);          /* wait for oprtr reply                */
if (args[0] != 'y')
  { errmsg("truncate aborted by operator request",cargs.cmd,"",0,0x21);
  }

#if defined(DWIN)              /* compiled on windows -DDWIN on cmd line*/
errmsg("truncate NOT supported on Windows version (uvhd.exe)"
       ,cargs.cmd,"",0,0x21);
ss = 0;                        /* - disable truncate on windows         */
#else
ss = truncate(fn1,cmn);        /* truncate file to specified size       */
#endif

if (ss < 0)
   errmsg("z (truncate) attempt returned fail status "
          ,cargs.cmd,fn1,0,0x21);

fileopen();                    /* re-open the file                    */

chkrcsz(rszo);                 /* check filesize/rcsz evenly divisible*/
fileptr = fileptr1;            /* reset fileptr to last rec in file   */

/*Oct08/11 - edit filesize for printf's %ld or %lld for 64/32bit HP gcc */
sprintf(fsedit,E64,filesize);

return(1);
}

/*eject*/
/*uvhd------------------------- findrec --------------------------------*/
/*May24/10 - add 'f' find specified rec#                                */
/*         - for variable lth records (RDW,LST,IDXf3/8)                 */
/* - assumes current rec# correct & searches forward                    */
/*May26/10 - add option 'm' & 'n' to find minimum to maximum recsize    */
/* f99         - use high# to see how many records in file (shows last) */
/* f999m123    - option 'm' to find record with specified record-size   */
/* f999m50n90  - option 'm' & 'n' to find recs from min to max size     */
/* ff          - repeat prior find command, save retype number & options*/
/*Nov21/10 - Index built automatically at program startup for varlth files*/

int findrec()
{
int mm;        /* option 'm' retrieved */
int nn;        /* option 'n' retrieved */

upnext = 0;    /*Jun02/10 - inhibit fp/rn updt */
vrtyp2 = '\0'; /*Sep15/17 - clear read unused bytes for prior record */

/*May26/10 - allow repeat cmd for find spcfd rec-size   */
if (cms[1] == 'f')       /* repeat prior find command ? */
  { upnext = 1;          /*Jun02/10 - set fp/rn upate   */
  }
else
  { fargs = cargs;       /* store initial find cmd args */
    /*Nov21/10 - decrement find rec# (now seems 0 rel)  */
    /* fargs.cmv--; */
    /*Jan10/11 - fix findrec not finding record#1 */
    if (fargs.cmv <= 1)
      { fargs.cmv = 1;
        recnum1 = 1;     /* init 1 since upnext 0 inhibits ++  */
        fileptr = 0;
      }
  }

mm = fargs.opi['m'-'a']; /* store option 'm' min size (exact of no max) */
nn = fargs.opi['n'-'a']; /* store option 'n' max size                   */

/*May19/11 - test recnum > records in file, ifso set fileptr to last rec*/
if (fargs.cmv >= rcount)
  { fargs.cmv = rcount;
    recnum1 = rcount;
    fileptr = fileptr1;
    upnext = 0;
    rsz1 = getrec(0);       /* get next record */
    upnext = 0;
    vrtyp2 = '\0';
    return(2);
  }

/*eject*/
/*Jul09/10 - added Index tables for findrec() faster access by record#  */
/*         - tables of fileptrs & record#s to 1,2,3% etc of filesize    */
/* if Index already build - use it to find target record# */
if (fii1 == 2)
  { /* lookup table of rec#s/fileptrs for 1st rec#/fileptr >= target rec# */
    for (fii = 1; fii < 102; fii++)
      { if (firn[fii] >= fargs.cmv)
          { fii--; 
            recnum1 = firn[fii];
            fileptr = fifp[fii];
            break;
          }
      }
  }

/* begin loop to get records until current rec# = spcfd rec# */
while (1)
  { rsz1 = getrec(0);       /* get next record */
    if (rsz1 <= 0)
      { strcpy(opmsg1,"EOF reached before find record# reached\n");
        strcpy(opmsg2,
        "enter -1 to see last in file, or 1 to return to begin file\n");
        vrtyp2 = '\0';
        return(-1);
      }

    /*May26/10 - test option m/n for recsize or recsize range */
    if ((mm) && (nn))
      { if ((rsz1 >= mm) && (rsz1 <= nn))
          { sprintf(opmsg1,"recsize %d (in range %d:%d) at rec# %d\n"
                         ,rsz1,mm,nn,recnum1);
            upnext = 0;           /* force display found record */
            vrtyp2 = '\0';
            return(1);
          }
      }
    else if (mm)
      { if (rsz1 == mm)
          { sprintf(opmsg1,"record-size %d found at record# %d\n",rsz1,recnum1);
            upnext = 0;           /* force display found record */
            vrtyp2 = '\0';
            return(1);
          }
      }

    /*Apr19/16 - '==' changed to '>=' in case of mistake */
    if (recnum1 >= fargs.cmv)     /* target rec# found ? */
      { upnext = 0;   
        vrtyp2 = '\0';
        return(1);
      }
  }
vrtyp2 = '\0';
return(0);
}

/*eject*/
/*----------------------------- findex ---------------------------------*/
/*Jul09/10 - build Index tables for findrec() faster access by record#  */
/*         - tables of fileptrs & record#s to 1,2,3% etc of filesize    */
/* initialize/calculate fifp[102] & firn[102] as follows: */
/* fifp[0]=0, fifp[1]=1% fsize, fifp[2]=2% fsize,..., fifp[100]=100% fisze*/
/* firn[0]=0, firn[1]=rec# at 1% filesize,..., firn[99]=lastrec# in file  */

int findex(void)
{
UVi64 fsa;               /* accumulate filesize for 1%, 2%,...,100%   */
UVi64 fs1;               /* calc 1% of filesize                       */
UVi64 fs1min;            /* min increment 64 or recsize if fixed      */
IndexBuild = 1;          /* force count recs for STL Index Build      */

/*Jul11/10 - add elapsed time msg at end indexing */
time_t time1;            /* time Indexing begun   */
time_t time2;            /* time Indexing ended   */
int    time3;            /* Indexing elapsed time */
int    ss,xx;
time1 = time(0);         /* get time at begining of Indexing */
fileptr = 0;             /* ensure fileptr at Begin File     */
upnext = 0;              /* inhibit recnum1++ on 1st get     */
recnum1 = 1;             /* init record ctr at begin file    */
/*Apr19/16 - recnum1 init 1 since upnext inhibits ++ on 1st get */
/*Apr19/16 - try recnum = 0 for RSV files */
/* if (ftype == 'v') */
/*   { recnum1 = 0;  */
/*   }               */

firn[0] = 1;             /* store rec#1 in table paired with fileptr 0 */
fsa = 0;                 /* init filesize acum for % points            */
fs1 = (filesize / 100);  /* calc 1% of filesize for increment to 100%  */

/*Nov21/10 - min filesize increment 500 bytes or filesize if < 500 */
/*Jan10/11 - chg to recsize if fixed or else 10 bytes (Apr22/11)   */
fs1min = 10;
if (ftype == 'f')
  { fs1min = rszo;
  }
if (fs1 < fs1min)
  { fs1 = fs1min;
  }
if (fs1 > filesize)
  { fs1 = filesize;
  }

/*Apr20/11 - test for Index already built for same filname & same filesize*/
ss = findexload();    /* returns 0 if Index has to be rebuilt */
if (ss)
  { return(2);
  }

/*eject*/
/*Jan10/11 - why fii=0 here inserting fileptrs, but fii=1 below insert rec#s*/
for (fii = 0; fii < 102; fii++)
  { /* store filesize %ptrs til filesize reached, fill rmndr with filesize*/
    if (fsa < filesize)
      { fifp[fii] = fsa;
      }      
    else
      { fifp[fii] = filesize;
      }
    fsa += fs1;          /* incremnt acum by 1% filesize         */
    /*Nov21/10 - inc fsa moved after storing to make 1st entry 0 */
  }

/* prepare for loop to get records & build Index */
fii = 1;           /* reset table index for build rec# phase */
fileptr = 0;       /* ensure fileptr reset to begin file     */
printf("Begin building Index to be saved in %s\n",finame);

/*eject*/
/* begin loop to get records until EOF */
while (1)
  { rsz1 = getrec(1);       /* get next record */
    if (rsz1 > 0)           /* if not EOF      */
      { /*Mar29/11 - store fileptr corspndng to recnum1 in 1% slots */
        /* if fileptr >= current fifp[fii] - store rec# & up index for next*/
        if (fileptr >= fifp[fii])
          { /*Apr19/16 - finding records 1 too high after count from 0 ? */
            firn[fii] = recnum1;
            fifp[fii] = fileptr;
            fii++;
          }
        /*May28/11 - test Index build progress message interval record count*/
        pmsgctr++;             /* increment Index build msg interval counter*/
        if (pmsgctr >= pmsgcnt)
          { printf("Index build at rec# %d of %s bytes filesize\n"
                   ,recnum1,fsedit);
            pmsgctr = 0;        /* reset ctr for next msg */
          }
      }

/*eject*/
    else
      { /* EOF - building Index for findrec() faster access by record# */
        /*Apr15/16 - kludge fix record count 1 too high for STL files  */
        /* - con't figure out why as of April 15/2016 ?                */
        /* - only for Indexed files with Alt Indexes, OK if no Alts ?  */
        /* if ((ftype == 'x') && (recnum1 > 0)) */
        /*   { recnum1--;                       */
        /*   }                                  */
        /* store last rec# in any remaining entries in Index table */
        while(fii < 102)
          { firn[fii] = recnum1;
            /*Apr15/16 - above recnum1 was rcount incorrect for STL*/
            /*Mar29/11 - store fileptr corspndng to EOF (filesize) */
            /*Apr19/16 - change to fileptr of last record */
            fifp[fii] = fileptrp;
            fii++;
          }
        /* set Index build switch off & set fileptr to last record   */
        fii1 = 2;            /* set Index build completed            */
        /*Jul11/10 get Index end time & calc elapsed time for opmsg1 */
        time2 = time(0);
        time3 = (time2 - time1);
        indexbuildtime = time3;   /* save for findexsave() */
        printf("Index built in %d seconds, saved in %s\n"
               ,time3,finame);
        /*Nov21/10 - store rcount & inhibit subsequent changes     */
        /* - set rcountx & eof1 here as well as at EOF in getrec() */
        rcount = recnum1;       /* store record count in file      */
        rcountv = rcount;       /* store record count in varlth file*/
        rcountx = 1;            /* inhibit subsequent calcrecs     */
        eof1++;                 /* count times reached EOF         */
        /*May19/11 - save ptr to last record in file               */
        fileptr1 = fileptrp;    /* store ptr to last rec in file   */
        /*Apr20/11 - save Index in a file to save rebuilding next time */
        findexsave();
        /*Oct08/16 - deleted count for displays from Index save/load */
        dcount2 = dcount1;      /* for deleted stats from index save/load*/
        dcount1 = 0;            /* - will reacum in getrecx (ignored) */
        /*may04/11 - init last BDW fileptr & rec# for any rdwerr() */
        bdwfp = 0;
        bdwrn = 1;
        return(1);
      }
  }
return(1);
}

/*eject*/
/*----------------------------- findexsave ------------------------------*/
/*Apr20/11 - save Index for loading next time uvhd called for this file  */
/*         - saved to $UVTMPDIR/uvhdindex_X------filename------X         */
/* - used if filesize same as when Index file written (saved in rec#2)   */

/* Index file records are formatted as follows                       */
/* #01:uvhdindex_X--------filename--------X                          */
/* #02:filesize=999999,fileptr1=999900,options=z8                    */
/* #03:IndexCreated=yymmdd_hhmmss,BuildTimeSeconds=999               */
/* #04:BDWmin=99,BDWmax=9999,BDWavg=999,Blocks=999                   */
/* #05:RCSZmin=99,RCSZmax=9999,RCSZavg=999,Records=9999,Deleted=9999 */
/* #06:1=0,2=99,3=999,4=999,...etc...                                */
/* #07:10=9999,11=9999,...etc...                                     */

int findexsave(void)
{
char *pp;
int ii,ss;
int ll;        /* length of accumulated rec#s=fileptrs in current rec */
int jj;        /* length of current rec#=fileptr          */
int nn;        /* line# ctr to create #04:...,#05:...,etc */

/* create filename & open file for writing                */
/*May03/11 - Index filename now formatted at program init */
/*         - $UVTMPDIR/uvhdindex_X---datafilename---X     */
fiptr = fopen(finame,"wb");       /* open file*/
if (fiptr == (FILE*)0)
  { errmsg("opening save Index file",finame,"",0,0x40);
  }

/* write 1st 5 records with filename, filesize,options,creation date, stats*/
strcpy(firec,fir1id);          /* #01:uvhdindex_ */
strcat(firec,fn1b);            /* basename of input file */
putlin0(fiptr,firec,180,'\n',0x42);
strcpy(firec,fir2fs);          /* #02:filesize=999,...   */

/*Oct08/11 - edit fileptr1 for printf's %ld or %lld for 64/32bit HP gcc */
sprintf(fpedit1e,E64,fileptr1);
sprintf(w1,"%s,fileptr1=%s,options=%s",fsedit,fpedit1e,opsu);
/*May19/11 - fileptr1 added for findexload to restore */

strcat(firec,w1);
putlin0(fiptr,firec,100,'\n',0x42);
sprintf(firec,"#03:IndexCreated=%s_%s,BuildTimeSeconds=%d"
             ,date6,time6,indexbuildtime);
putlin0(fiptr,firec,100,'\n',0x42);

/* write rec #04:BDWmin=99,BDWmax=99,BDWavg=99,Blocks=99           */
/* calc bdwavg & rdwavg from totals accumulated during index build */
if (bdwcnt)
  { bdwavg = (bdwsum / bdwcnt);
  }
if (recnum1)
  { rdwavg = (rdwsum / recnum1);
  }

/*eject*/
sprintf(firec,"#04:BDWmin=%d,BDWmax=%d,BDWavg=%d,Blocks=%d"
             ,bdwmin,bdwmax,bdwavg,bdwcnt);
putlin0(fiptr,firec,100,'\n',0x42);
sprintf(firec,"#05:RCSZmin=%d,RCSZmax=%d,RCSZavg=%d,Records=%d,Deleted=%d"
             ,rdwmin,rdwmax,rdwavg,recnum1,dcount1);
putlin0(fiptr,firec,100,'\n',0x42);

/* begin loop to write rec#s=fileptrs */
/* - restricting each record to < 80 bytes */
memset(firec,'\0',180);        /* init build area */
nn = 6;                        /* init ctr for #05:,#06: etc */
sprintf(firec,"#%02d:",nn);

for (ii=0; ii < 102; ii++)
  { /* format current rec#=filptr & test to see if appending exceeds 80 */
    sprintf(w2,"%d=%ld,",firn[ii],fifp[ii]);
    jj = strlen(w2);           /* get lth of current rec#=filptr     */
    ll = strlen(firec);        /* get acum lth of rec#s=fileptrs,... */
    if (ll+jj >= 80)
      { putlin0(fiptr,firec,100,'\n',0x42);
        memset(firec,'\0',100);
        nn++;
        sprintf(firec,"#%02d:",nn);
      }
    strcat(firec,w2);

    /* quit when index table fileptr entry reaches filesize */
    if (fifp[ii] >= filesize)
      { break;
      }
  }

/* write any partial line */
if (firec[0])
  { putlin0(fiptr,firec,100,'\n',0x42);
  }

/* close file & return */
fclose(fiptr);
return(1);
}

/*eject*/
/*----------------------------- findexload ------------------------------*/
/*Apr20/11 - load Index from a prior uvhd for this file             */
/*         - if index file exists with matching filename & filesize */

/* called at begin findex() */
/* - return 1 if index usable (saves rebuilding) */
/* - return 0 if index must be built             */

/* Index file records are formatted as follows                       */
/* #01:uvhdindex_X--------filename--------X                          */
/* #02:filesize=999999,fileptr1=999900,options=z8                    */
/* #03:IndexCreated=yymmdd_hhmmss,BuildTimeSeconds=999               */
/* #04:BDWmin=99,BDWmax=9999,BDWavg=999,Blocks=999                   */
/* #05:RCSZmin=99,RCSZmax=9999,RCSZavg=999,Records=9999,Deleted=9999 */
/* #06:1=0,2=99,3=999,4=999,...etc...                                */
/* #07:10=9999,11=9999,...etc...                                     */

int findexload(void)
{
char *pp;
int ii,jj,kk,ss;
char firns[16];    /* current rec# string extracted from current line    */
char fifps[16];    /* current fileptr string extracted from current line */
int firn1;         /* current rec# converted to int for load to table    */
UVi64 fifp1;       /* current fileptr converted to long for load to table*/
/*May19/11 - fix index not working fileptr > 2.1 gig, chg fifp1 to UVi64 */
char filn[8];      /* current line# "#99:" validate index file records   */
int firi;          /* dsplcmnt to current rec#=fileptr on current line   */

/* force index rebuild even if existing & same size */
if (opsi['f'-'a'] & 0x02)
  { return(0);
  }

/* test for index file existing for this filename & same filesize */
/*May03/11 - Index filename now formatted at program init */
/*         - $UVTMPDIR/uvhdindex_X---datafilename---X     */
ss = stat(finame,&fstatI);
if (ss < 0)
  { /* stat failed, index does not exist, return 0 to build now */
    return(0);
  }

/* open file, get filesize when index built & compare to current filesize*/
fiptr = fopen(finame,"rb");  /* open file*/
if (fiptr == (FILE*)0)
  { errmsg("opening saved Index file",finame,"",0,0x40);
  }

/*eject*/
/* get filesize from rec#2 when index built & compare to current filesize*/
ss = getlin0(fiptr,firec,180,'\n',0x0562);
ss = getlin0(fiptr,firec,180,'\n',0x0562);
if (memcmp(firec,fir2fs,13) != 0)   /* must be #02:filesize= */
  { return(0);
  }

/* force rebuild if filesize not same */
stncopy(filesizeIs,firec+13,',',16,0x03);
filesizeI = strtoll(filesizeIs,(char**)0,10);
if (filesizeI != filesize)
  { return(0);
  }
/*Oct06/16 - force rebuild if options not same */
pp = strstr(firec,"options=");
if (pp)
  { stmcopy(fileops1,pp+8,", ",20,0x03);
    if (strcmp(fileops1,runops) != 0)
      { return(0);
      }
  }

/*May19/11 - restore ptr to last record in file */
pp = strstr(firec,"fileptr1=");
if (pp)
  { stncopy(fileptr1s,pp+9,',',20,0x03);
    /* fileptr1 = atol(fileptr1s); Oct04/11 chg back gcc HP Itanium */
    fileptr1 = strtoll(fileptr1s,(char**)0,10);
  }

/* save create date from rec#03 for oprtr msg */
ss = getlin0(fiptr,firec,180,'\n',0x0562); /* get record#03 */
pp = strstr(firec,"Created=");
if (pp)
  { stncopy(indexcreated,pp+8,',',20,0x03);
  }

/*eject*/
/* get rec#04 - save BDWmin if filetype z8 (validate BDW bypass on getrdw)*/
ss = getlin0(fiptr,firec,180,'\n',0x0562);
pp = strstr(firec,"BDWmin=");
if (pp)
  { stncopy(bdwmins,pp+7,',',16,0x03);
    bdwmin = atol(bdwmins);
  }
if ((opsi['z'-'a'] & 0x08) && (bdwmin <= 8))
  { errmsg("load index rec#04 BDWmin 0 for filetype z8",finame,"",0,0x21);
  }

/*May11/11 - get rec#05 RCSZmin,RCSZmax,RCSZavg,Records & save rcount */
ss = getlin0(fiptr,firec,180,'\n',0x0562);
pp = strstr(firec,"Records=");
if (pp)
  { stncopy(rcounts,pp+8,',',16,0x03);
    rcount = atol(rcounts);  /* save record count */
    rcountx = 1;             /* inhibit calcrecs */
  }

/*Oct08/16 - add deleted count to displays & Index file save/load */
pp = strstr(firec,"Deleted=");
if (pp)
  { stmcopy(dcounts1,pp+8,", ",16,0x03);
    dcount1 = atol(dcounts1);  /* save record count */
  }

/*May28/11 - save max recsize for search op max check */
pp = strstr(firec,"RCSZmax=");
if (pp)
  { stncopy(rdwmaxs,pp+8,',',16,0x03);
    rdwmax = atol(rdwmaxs);  /* save recsize max */
  }

printf("Begin ReLoad Index %s, built %s\n",finame,indexcreated);

/*eject*/
/* begin loop to load index file rec#s=fileptrs into index table */
/* - until EOF reached in Index file */
ii = 0;        /* init index to tables of rec#s & fileptrs     */
jj = 5;        /* init ctr to verify index file records "#99:" */
while (1)
  { memset(firec,'\0',100);
    ss = getlin0(fiptr,firec,180,'\n',0x0562);
    if (ss < 0)
      { break;
      }
    jj++;            /* increment line# */
    sprintf(filn,"#%02d:",jj);
    if (memcmp(firec,filn,4) != 0)
      { return(0);
      }

    /* begin loop to store rec#s=fileptrs from current record */
    /* - presume end of line when nulls or rec# < 1           */
    firi = 4;          /* init dsplcmnt to 1st rec#=fileptr   */
    while(firec[firi])
      { sdscopy(firns,firec,100,&firi,"~~=~",0x12);
        sdscopy(fifps,firec,100,&firi,"~~,~",0x12);
        firn1 = atol2(firns,'~',16,0x01);
        /* fifp1 = strtol(fifps,(char**)0,10); May19/11*/
        /* fifp1 = atol(fifps); Oct04/11 change back for 32bit gcc HP Itanium */
        fifp1 = strtoll(fifps,(char**)0,10);
        if (firn1 < 1)
          { break;
          }
        firn[ii] = firn1;
        fifp[ii] =fifp1;
        ii++;
      }
  }

/* EOF on Index file of rec#s=fileptrs */
/* - fill remaining table entries with last rec#=filptr */
ii--;  /* backup to last entry */
for (jj=ii; jj < 102; jj++)
  { firn[jj] = firn[ii];
    fifp[jj] = fifp[ii];
  }

fclose(fiptr);
/* inform oprtr - using existing Index file */
printf("ReLoaded Index %s, built %s\n",finame,indexcreated);
fii1 = 2;               /* set Index reload completed        */
rcountv = firn1;        /* store record count in varlth file */
return(1);
}

/*eject*/
/*---------------------------- byte2rec --------------------------------*/
/*Aug23/11 - convert fileptr to record#, when uvhd entered from uvhdcob */
/*         - uvhdcob exit passes fileptr byte# on option 'b'            */
/*         - this subrtn called if optn z RDW file                      */
/*         - to lookup index tables of byte#s & record#s                */

int byte2rec(UVi64 fp1)
{
int fii;

upnext = 0;    /* inhibit fp/rn updt */

/* if Index already built - use it to find target record# */
if (fii1 == 2)
  { /* lookup table of rec#s/fileptrs for 1st rec#/fileptr >= target rec# */
    for (fii = 1; fii < 102; fii++)
      { if (fifp[fii] >= fp1)
          { fii--; 
            fileptr = fifp[fii];
            recnum1 = firn[fii];
            break;
          }
      }
  }

/* begin loop to get records until current rec# = spcfd rec# */
while (1)
  { rsz1 = getrec(0);       /* get next record */
    if (rsz1 <= 0)
      { strcpy(opmsg1,"EOF reached converting uvhdcob byte# to record#\n");
        strcpy(opmsg2,
        "enter -1 to see last in file, or 1 to return to begin file\n");
        return(-1);
      }

    if (fileptr == fp1)     /* filptr = fp passed from uvhdcob ? */
      { upnext = 0;   
        return(1);
      }
  }
return(0);
}

/*eject*/
/*uvhd------------------------- switchx -------------------------------*/
/* return record slot size from ptr to 2 or 4 byte rec hdr             */
/* switch big-end to little-end if INTEL, SCO (not SUN, HP, etc)       */
/* - stores record-type, record-size,& slot-size                       */
/* - calc slot size = rcsz + 2/4 rechdrsize + roundup multi 4 or 8     */
/* - arg2 indicates 2 or 4 byte rec hdr                                */
/* - apply max & min rules to vrsize & vrslot                          */
/* - switchx return value used for min & max at file open              */
/*Feb23/07 - fix read IDXFORMAT8 switchx roundup multiple of 4 not 8   */
/*Feb21/16 - add bits 0x01 to inhibit adding rechdrsize to slotsize    */
/*Feb26/16 - change proto to 4 args, bits inhib +hdrsz now arg4 separate*/

int switchx(char *vhdr, int vrhs, int vrup48, short bits)
{
int mm;
memset(u1.cc,'\0',4);          /* clear work area union                */

if (vrhs == 4)                 /* if input rec hdr 4 vs 2 ?            */
  { memcpy(u1.cc,vhdr,4);      /* store 4 byte rec type & size         */
    vrtyp = u1.cc[0];          /* isolate rec type                     */
    vrtyp2 = vrtyp;            /* save vrtyp for unused space check    */
    vrtyp &= 0xF0;             /* clear digit (type in zone only)      */
    /* limit max rcsz to 65K (clear 1st 2 bytes of 4 byte rec size)    */
    u1.cc[0] = 0x00;
    /* u1.cc[1] = 0x00; */
    /*May17/18 - change max recsize to 20 bits as follows: */
    u1.cc[1] &= 0x0F;
  }
else if (vrhs == 2)
  { memcpy(u1.cc+2,vhdr,2);    /* store 2 byte rec type & size         */
    vrtyp = u1.cc[2];          /* isolate rec type                     */
    vrtyp2 = vrtyp;            /* save vrtyp for unused space check    */
    vrtyp &= 0xF0;             /* clear digit (type in zone only)      */
    u1.cc[2] &= 0x0F;          /* clear zone (type) from rcsz          */
  }
else
  { errmsg("switchx arg2 not 2 or 4 (rechdrsize)"," "," ",0,0x21);
  }

/* switch big-end to little-end if INTEL or SCO (not SUN, HP, etc)     */
#if (LEM)
    u2.cc[0] = u1.cc[3];
    u2.cc[1] = u1.cc[2];
    u2.cc[2] = u1.cc[1];
    u2.cc[3] = u1.cc[0];
#else
    memcpy(u2.cc,u1.cc,4);
#endif
vrsize = u2.ii;                /* store record size                  */

/*eject*/
/* calc slot size = rcsz + rechdrsize (2/4) + roundup to multiple 4  */
/*Feb21/16 - add bits 0x01 to inhibit adding rechdrsize to slotsize  */
if (bits & 0x01)
  { vrslot = vrsize;
  }
else
  { vrslot = vrsize + vrhs;
  }

if (vrup48)
  { mm = (vrslot % vrup48);         /* calc modulus 4/8  */
    if (mm)
      { vrslot += (vrup48 - mm);    /* round up multiple of 4/8 */
      }
  }
/* calc rec data size + any fill, but not rechdr             */
/* vdsize = vrslot - vrhs;  <-- calc data size with any fill */
/*Feb26/16 - used in uvhdcob, NOT  used here in uvhd         */

return(u2.ii);
}

/*eject*/
/*uvhd----------------------- open file -------------------------------*/
/* open input file, get file status, file size,& modification date     */
/* - coded as subfunction, since also used by 'z' truncate command     */
/* if filesize zero, set high - to allow uvhd on tape files            */

int fileopen(void)
{
/* 1st get file size, using 'stat' function */
/*Dec07/07 - for win32 (lcc or gcc/MinGW), use _stati64 */
#if defined(LF64) && defined(DWIN)
statstat = _stati64(fn1,&fstat1);
#elif defined(LF64)
statstat = stat64(fn1,&fstat1);
#else
statstat = stat(fn1,&fstat1);
#endif

if (statstat < 0)
     errmsg("file status error",fn1,"",0,0x40);

filesize = fstat1.st_size;       /* store filesize                     */

/*Mar04/14 - following disabled, tapes obsolete */
/* allow tape files by setting filesize high if stat returns 0 */
/* if (filesize <= 0)       */ 
/*  { filesize = 999999999; */
/*  }                       */

/*Mar04/14 - save filesize on open to compare to filesize on close */
if (filesizeopn == 0)
  { filesizeopn = filesize;
  }

/*Jun12/06 - ensure BDWmin not saved from last block in file */
filesizeb = (filesize - 32768);  /* calc fileptr of last max blocksize */
if (filesizeb <= 0)
  { filesizeb = filesize;
  }

cnvdttm(fmdate,fstat1.st_mtime); /* store last mod date                */
fmdate[10] = '\0';               /* drop seconds                       */

/* set file flags depending on option u (update)                       */
if (opsc['u'-'a'])
    openflags = O_RDWR;          /* presume O_RDWR (2) update ?        */
else
    openflags = O_RDONLY;        /* no optn u - change O_RDONLY (0)    */

/*Mar2003 - DWIN version requires O_BINARY flag                        */
#ifdef DWIN
openflags |= O_BINARY;           /* add binary flag for lcc/gcc/MinGW  */
#endif

/*eject*/
/* open file                                                           */
/*May2001 - provide largefile support -D LF64                          */
/*        - fopen changed to open/open64 for lseek/lseek64 (no fopen64)*/
/*Dec07/07 - open64 N/R for lcc/gcc/MinGW */
#if defined(LF64) && !defined(DWIN)
fd1 = open64(fn1,openflags,0);
#else
fd1 = open(fn1,openflags,0);
#endif

if (fd1 < 0)
  {  errmsg("cant open file",fn1,"",0,0x40);
  }

/*Dec02/11 - disable filelock for MinGW */
#if ! defined(DWIN)
/*Dec07/10 - setlock on fileopen if option u (but not u2)   */
if ((opsc['u'-'a']) && (!(opsi['u'-'a'] & 0x02)))
  { /* request file-lock via subfunction (calling fcntl)        */
    /* - if file locked, display status, wait 10 seconds & retry*/
    while (1)
      { lockstat = setlock(fd1,F_WRLCK, fn1);
        if (lockstat == 0)
          { waitlock(&flwait1,fn1,"lock on open file");
            continue;
          }
        else
          { break;
          }
      }
  }
#endif

/*Apr05/11 - init fileptr1 & recnum1 now in fileopen() for drop cmd reopn*/
fileptr1 = filesize;   /* init fileptr1, set (filesize-recsize) at EOF   */
recnum1 = 1;           /* init record# for display */
/*Apr19/16 - init 0 (vs 1) var lth counts now in varlth get subrtns */
/*May03/11 - reset fileptr in case of drop re-open */
fileptr = 0;
fileseek("fileopen");
upnext = 0;  /* ensure no fileptr increment on 1st getrec()    */
rsz1p = 0;   /* ensure rsz1p 0 for 1st getrec upfileptr(rsz1p) */
bdwnext = 0; /* so getrdw will recognize 1st BDW at begin file */

/*Oct08/11 - edit filesize for printf's %ld or %lld for 64/32bit HP gcc */
sprintf(fsedit,E64,filesize);

return(1);
}

/*eject*/
/*---------------------------- fileclose ------------------------*/
/*Mar04/14 - get filesize on close & compare to open filesize    */
/*         - warn if different (valid only if delete or trucnate */
int fileclose(void)
{

close(fd1);          /* close file */

#if defined(LF64) && defined(DWIN)
statstat = _stati64(fn1,&fstat1);
#else
statstat = stat64(fn1,&fstat1);
#endif

filesizecls = fstat1.st_size;       /* store filesize on close */

if (filesizecls != filesizeopn)
  { opcmd[0] = 'n';
    while (opcmd[0] != 'y')
      { printf("closing filesize %ld NOT= opening filesize %ld\n"
               ,filesizecls,filesizeopn);
        printf("OK if d(delete) or z(truncate), enter 'y' to acknowledge ");
        fgets(opcmd,80,stdin);
      }
  }

return(1);
}
    
/*eject*/
/*----------------------------- setlock --------------------------------*/
/* set file lock on sequential file                                     */
/* - called after fopen (passed fileptr to convert to file dscrptr)     */
/* - return 0 if file already locked                                    */
/* - return 1 if set file lock successful (not already locked)          */

int setlock(int fd, int locktype, char *fname)
{
/*Dec02/11 - disable filelock for MinGW */
#if ! defined(DWIN)
struct flock flarg;            /* file lock arguments structure         */

/* setup flock values for fcntl call - to lock entire file              */
flarg.l_type = locktype;
flarg.l_whence = 0;
flarg.l_start = 0;
flarg.l_len = 0;
flarg.l_pid = 0;

/* request filelock via fcntl in while loop                         */
/* - repeat request if interrupt or deadlock                        */
/* - if file already locked, return 0 to caller                     */
/* - if error other than deadlock/already locked -> errmsg & end    */
/* - if filelock request successful, return 1 to caller             */

while (fcntl(fd,F_SETLK,&flarg) == -1)
  { if (errno == EINTR || errno == EDEADLOCK || errno == EDEADLK)
        continue;
    if (errno == EACCES || errno == EAGAIN)
        return(0);                   /* return 0 for already locked */
    else
        errmsg("setlock fcntl failed",fname,"",0,0x40);
  }
#endif
return(1);                           /* return 1 for setlock success */
}

/*eject*/
/*---------------------------- waitlock --------------------------------*/
/* wait on filelock & display status message every 10 secs +            */
/* wait period = 10 * (ttl mins waiting already) in seconds             */
/*             = minimum 10 secs, max 300 secs (5 mins)                 */

void waitlock(int *waita, char *file, char *msg)
{
/*Dec02/11 - disable filelock for MinGW */
#if ! defined(DWIN)
int waitm;
int wait1;

/* display file lock status message                                    */
printf("waiting %d seconds for %s %s\n",*waita,msg,file);

/* calc sleep time wait1 = (10 * minutes waiting) seconds              */
/* minimum 10 secs, max 300 secs                                       */
waitm = *waita / 60;                /* calc wait time in minutes       */
wait1 = 10 * waitm;                 /* calc secs to wait this time     */
if (wait1 < 10) wait1 = 10;         /* min 10 secs                     */
if (wait1 > 300) wait1 = 300;       /* max 300 secs                    */
sleep (wait1);                      /* sleep current wait time         */
*waita += wait1;                    /* acum total wait time            */

#endif
return;
}

/*eject*/
/*uvhd----------------------- fileseek --------------------------------*/
/* May2001 - fileseek calls lseek or lseek64                           */
/* - always seeks to fileptr variable                                  */
/* - returns only if no error detected                                 */

int fileseek(char *emsg)
{
UVi64 fileptrseek;

/*Nov21/10 - ensure fileptr not negative */
/*Feb26/19 - reset recnum0/recnum1 on fileptr==0 as well as fileptr<0 */
if (fileptr <= 0)
  { fileptr = 0;
    recnum0 = 0;
    recnum1 = 1;
    /*Apr19/16 - recnum1 inits restored to 1 (not 0) */
  }

/* May 2001 - for largefiles, use lseek/lseek64 vs fseek           */
/* status = fseek(fp1,fileptr,0);    <-- fseek uses file PTR       */
/* status = lseek(fd1,fileptr,0);    <-- lseek uses file DSCRPTR   */
/* status = lseek64(fd1,fileptr,0);  <-- lseek uses file DSCRPTR   */

/*May9/2001 - having problems debuggung for largefiles             */
/* - for debug: add msg below displayed on lseek64 ??              */
/* sizeoffp = sizeof(fileptr); */
/* printf("DEBUG: lseek64 fileptr=%ld, sizeoffileptr=%d\n",fileptr,sizeoffp);*/

/*Dec07/07 - for lcc/gcc/MinGW, call lseeki64             */
/*Dec07/07 - we bomb out when we reach the 2 gig limit    */
/*Dec09/07 - fixed by new lseeki64 modified from fseeki64 */
fileptrseek = fileptr;     /* debug break point */

#if defined(LF64) && defined(DWIN)
/*Dec02/11 - change lseeki64 to _lseeki64 for MinGW) */
seekstat = _lseeki64(fd1,fileptr,0);
#elif defined(LF64)
seekstat = lseek64(fd1,fileptr,0);
#else
seekstat = lseek(fd1,fileptr,0);
#endif

if (seekstat < 0)
  { printf("seek error at %ld\n",fileptr);
    errmsg("seek error",fn1,emsg,0,0x40);
  }

return(1);
}

/*Feb04/09 - cmd history code removed (saved in src/uvhd_history.c)  */
/*         - 300 lines of code (histcode) removed                    */
/*         - sorthtdd, bublsort, inithist, writehist                 */

/*eject*/
/*uvhd-------------------------- openfileX ------------------------------*/
/*Feb05/09 - subfunction to open output files P,I,W,V,D                  */

int openfileX(char *xfname, char *xfid, FILE **xfptr, int *xfopn, short bits)
{
/* inhibit if file already open */
if (*xfopn)
  { return(0);
  }

/* base filename created at program init - now add time & ID (P/I/W/V/D) */
/* $UVTMPDIR/inputfilename_yymmdd_        - will append time & P/I/W/V/D */
/* $UVTMPDIR/inputfilename_yymmdd_hhmmssW - example for Write command    */

getdatetime();                /* get current date & time */

/* create tmp dir if not already present */
/*May03/11 - moved to program init       */

/* create filename & open file */
strcpy(xfname,basename);      /* base filename       */
strcat(xfname,time6);         /* append current time */
strcat(xfname,xfid);          /* append file ID      */

*xfptr = fopen(xfname,"wb");
if (*xfptr == (FILE*)0)
  { errmsg("opening file",xfname,"",0,0x40);
  }

/* ifptr = *xfptr;   Feb05/09 temp DEBUG */

*xfopn = 1;                  /* set file open switch */
return(1);
}

/*eject*/
/*uvhd-------------------------- closefileX ------------------------------*/
/*Feb05/09 - subfunction to close output files P,I,W,V,D                  */
/*Dec29/10 - add bits 0x01 to cause errmsg if file not open               */

int closefileX
   (char *xfname, char *xfid, FILE **xfptr, int *xfopn, int *xfctr, short bits)
{
/* test switch & close file if only if open */
if (*xfopn)
  { fclose(*xfptr);
    printf("%s closed, %d records written\n",xfname,*xfctr);
    *xfctr = 0;            /* clear output count for next cmd */
  }
else if (bits & 0x01)
  { printf("%s No File Open for close attempt\n",xfname);
  }

*xfopn = 0;                /* reset file open switch */
return(1);
}

/*-------------------------- close files ---------------------------*/
/*Dec29/10 - new cmd 'l' to close files                             */
/* lw  - close write file                                           */
/* lp  - close print file                                           */
/* lvp - close verify print file                                    */
/* lvw - close verify write file                                    */

int close1(void)
{
if (cops[1] == 'w')
  { closefileX(wfname,"W",&wfptr,&wfopn,&wfctr,0x01);
  }
else if (cops[1] == 'p')
  { closefileX(pfname,"P",&pfptr,&pfopn,&pfctr,0x01);
  }
else
  { printf("invalid modifier (2nd/3rd chars) for close command (l)\n");
    printf("lw=writefile, lp=printfile, lvp=verifyprint, lvw=verifywrite");
  }

return(0);
}

/*eject*/
/*uvhd--------------------------- calcrsl --------------------------------*/
/* calc possible record size by decrementing from current recsize         */
/* - until we can divide recsize evenly into filesize                     */

int calcrsl(int rsiz, int fsiz)
{
int rsiz1;
int mod1;

/* inhibit calc if recsize <= 0 or >= filesize */
if ((rsiz <= 0) || (rsiz >= fsiz))
  { return (rsiz);
  }

for (rsiz1=rsiz; rsiz1 > 0; rsiz1--)
  { mod1 = (fsiz % rsiz1);
    if (mod1 == 0)
      { return(rsiz1);
      }
  }

return(0);
}

/*uvhd--------------------------- calcrsg --------------------------------*/
/* calc possible record size by incrementing from current recsize         */
/* - until we can divide recsize evenly into filesize                     */

int calcrsg(int rsiz, int fsiz)
{
int rsiz1;
int rsiz2;
int mod1;

/* inhibit calc if recsize <= 0 or >= filesize */
if ((rsiz <= 0) || (rsiz >= fsiz))
  { return (rsiz);
  }

rsiz2 = rsiz * 4;                 /* calc upper limit for increment test */

for (rsiz1=rsiz; rsiz1 <= rsiz2; rsiz1++)
  { mod1 = (fsiz % rsiz1);
    if (mod1 == 0)
      { return(rsiz1);
      }
  }

return(0);
}

/*eject*/
/*uvhd-------------------------- printRV ------------------------------*/
/* display warning message in Reverse Video                            */

short printRV(char *warnmsg)
{

/*build Reverse Video warnmsg, allowing for inhibit via option i2 */
/*Jan10/11 - option chg to i from b now used for Begin display fileptr */
warnmsg2[0] = '\0';             /* init out area for inhibit option */
if ((opsi['i'-'a'] & 0x02) == 0)
    strcat(warnmsg2,smso);
strcat(warnmsg2,warnmsg);
if ((opsi['i'-'a'] & 0x02) == 0)
    strcat(warnmsg2,rmso);
printf("%s",warnmsg2);

return(1);
}

/*eject*/
/*uvhd---------------------------- showhelps ----------------------------*/
/* show helpscreens - arg1 points to a table of helpscreen pointers      */
/* - default to showing each in sequence until null pointer (end table)  */
/*   (see table of help screen ptrs stored up front in this program)     */
/* - allow user to enter help screen# to modify display sequence         */
/*   (or 'q' to quit help screens, 'qq' to quit help screens & program   */

int showhelps(char **hsp[], int hsmax)
{
int ii = 0;              /* index for table of help screen ptrs     */
int rr = 0;              /* user reply for next desired help screen */
char reply[80];          /* operator reply                          */

/* begin loop to display help screens until end of table, or user quit */
while (ii < hsmax)
  { showhelp(hsp[ii],30,0x01);    /* show current help screen          */
    if (ii < hsmax-1)
    { printf("enter null for next help screen, or help screen#, or q=quit--> ");
    }
    else
     { printf("End %d help screens, enter desired#, or Null to exit help --> "
           ,hsmax);
     }
    fgets(reply,80,stdin);     /* wait for operator reply           */
    if (reply[0] == 'q')
      { return(2);             /* return 2 to indicate oprtr quit   */
      }

    /* if operator entered valid index to desired help screen       */
    /* - use it, else display next sequential help screen           */
    rr = atol(reply);          /* convert reply to integer          */
    rr--;                      /* decrement for zero relative index */
    if ((rr >= 0) && (rr < hsmax))
      { ii = rr;               /* replace table index with users desire */
      }
    else
      { ii++;                  /* increment to next sequential help screen*/
      }
  }

return(1);
}

/*eject*/
/*Dec19/10 subfnctns for getop, putop1, etc to suport acum & move numeric*/
/*------------------------------- getop -------------------------------*/
/* retrieve op input to a common format (binary UVi64)                  */
/* argument is operand# coded 0-7 for ops 1-8                          */
/* - op area may be record input, record output, work area,or constant */
/*   (opptr already determined by calling function)                    */
/* - input may be various types: zoned/packed/binary,ascii/ebcdic      */

/*Dec19/10 - add getop,putop1,etc to support acum & move numeric */
/*         - arg changed from op# (0-7) to ptr to cargs structure*/
/*Dec26/10 - arg3 bits added to specify op2 for move numeric     */

UVi64 getop(char *recd, struct CA *carg, short bits)
{
UVi64 fill1 = 0;              /* assist debug                         */
UVi64 wbin;                   /* retrieval & return w/s               */
UVi64 fill2 = 0;              /* may help if gen I32 & user spcfs >4b */
UVi32 wint;                   /* retrieval for 4 byte int             */
short wshort;                 /* retrieval for 2 byte short           */
int ii;
Uchar wsign;
float wflt;                    /* retrieval w/a for float & double    */
double wdbl;

/* following local variables to simplify coding in this function      */
int opdsp;
int oplth;
char  *opptr;
char  opdt;
char  opdc;
Uchar *wbinpi;
Uchar *wbinp = (Uchar*)&wbin;
Uchar *wintp = (Uchar*)&wint;
int w2b;                       /* calc begin byte data in w/a w2      */
wbin = 0;                      /* clear retrieval work UVi32/UVi64    */

/* 1st store field ptr & lth locally to simplify coding this function*/
/*Dec19/10 - getop arg now ptr to cargs structure                */
/*Dec26/10 - arg3 bits added to specify op2 for move numeric     */
if (bits & 0x02)
  { opdsp = carg->a2d;
    opptr = recd + carg->a2d;
    oplth = carg->a2l;
    opdt = carg->a2dt;
    opdc = carg->a2dc;
  }
else
  { opdsp = carg->a1d;
    opptr = recd + carg->a1d;
    oplth = carg->a1l;
    opdt = carg->a1dt;
    opdc = carg->a1dc;
  }

/*eject*/
/* if operand 0 lth dont attempt access, return 0 */
if (carg->a1l <= 0)
  { return 0;
  }

/* setup ascii/ebcdic codes for digits & signs depending on op data type*/
setcodes(opdc);

/* calc indices for begin byte of op when processed in work area       */
w2b = (20 - oplth);

/* test op type & retrieve data accordingly                          */
/* 1st test for op numeric count (stored in dsplcmnt as binary value)*/
/*dec26/10 - assume numeric value if no length                       */
if (!oplth)
  { wbin = opdsp;
    return(wbin);
  }

/* must be area adrs - test for 'binary' data                        */
if (opdt == 'b')                            /* binary ? (1,2,or 4)   */
 { /* if binary field NEG - init retrieval area to all x'FF's        */
   /* - in case input fewer bytes than UVi64 4/8 architecture        */
   /* test left byte for BEM & right byte for LEM                    */
   if (BEM)
     { wsign = (Uchar) opptr[0];       /* get left byte to test BEM neg */
     }
   else
     { wsign = (Uchar) opptr[oplth-1]; /* get right byte to test BEM neg*/
     }
   if (wsign & 0x80)
     { wbin = -1;
     }
   else
     { wbin = 0;
     }

   /* now test for binary field length & convert accordingly          */
   if (oplth == 1)                          /* binary 1 byte char ?   */
     { wbin = opptr[0];                     /* store in 32/64 integer */
     }

   else if (oplth == 2)                     /* binary short ?         */
     { memcpy((Uchar*)&wshort,opptr,2);     /* retrieve to short w/a  */
       if (opdc == 's')
         { bswap2((Uchar*)&wshort);         /* switch big/little ends */
         }
       wbin = wshort;                       /* store in common 4/8    */
     }

/*eject*/
   else if (oplth == 3)                     /* binary 3 bytes ?      */
     { /* justify for BEM/LEM in 4 byte int, then test for byte swap */
       /* Dec2001 - byte swap test added (was ignored for 3 byte binary)*/
       if (BEM)
         { memcpy(wintp+1,opptr,3);         /* get right 3 bytes     */
           if (opdc == 's')
             { bswap3b(wintp);              /* switch 3 bytes BEM    */
             }
         }
       else
         { memcpy(wintp,opptr,3);           /* get left 3 bytes      */
           if (opdc == 's')
             { bswap3i(wintp);              /* switch 3 bytes INTEL  */
             }
         }
       wbin = wint;                         /* store in common 4/8    */
     }

   else if (oplth == 4)                     /* binary 4 bytes ?       */
     { memcpy(wintp,opptr,4);               /* retrieve to 4 byte int */
       if (opdc == 's')
         { bswap4(wintp);                   /* switch big/little ends */
         }
       wbin = wint;                         /* move/justify in 4/8 bytes*/
     }

/* length 5-8 */
   else
     {
#if (BEM)
       /* I64 & BEM copy lths 5-8 right justified into zero work area  */
       wbinpi = (wbinp+8-oplth);
       memcpy(wbinpi,opptr,oplth);
#else
       /* I64 & LEM copy lths 5-8 left justified into zero work area   */
       memcpy(wbinp,opptr,oplth);
#endif
       /* I64, wbin stored for BEM or LEM, now byte swap if option set */
       if (opdc == 's')
         { bswap8(wbinp);
         }
     }
 }

/*eject*/
/* test op data type for 'zoned'                                        */
/* ifso - retrieve right adjusted into zero filled work area            */
/*      - test for sign, set sign switch,& remove sign from data        */
/*      - then convert to common format (binary 32/64)                  */
else if (opdt == 'z' || opdt == 'c')
 { memset(w2,zero,20);                /* zero fill w/a ascii/ebcdic*/
   w2[20] = '\0';
   memcpy(&w2[w2b],opptr,oplth);
   tstsignz(w3,w2,carg);              /* test sign, set sw, remove */

/* convert numeric string to integer long or long long depending I32/I64*/
   /* wbin = strtoll(w3,(char**)0,10); May19/11*/
   /* wbin = atol(w3); Oct04/11 change back for 32bit gcc HP Itanium */
   wbin = strtoll(w3,(char**)0,10);

   /*Jun00 - option t2 (inhibit recognizing '-' as neg) added in tstsignz*/
   /* - might still be a problem if atoi & strtoll recognize '-' as neg  */
   /*   & they do, BUT only if leading & trailing was my problem OK now  */
   if (sign == '-')
     { wbin = wbin * (-1);                 /* set neg if indicated      */
     }
 }

/* test op data type for 'packed'                                       */
/* ifso - retrieve right adjusted into zero filled work area            */
/*      - test for sign, set sign switch,& remove sign from data        */
/*      - then unpack & convert to common format (binary 32/64)         */
else if (opdt == 'p')
 { memset(w2,'\0',21);                /* null fill for packed retrieve */
   memcpy(&w2[w2b],opptr,oplth);
   tstsignp(w2,carg);   /* test sign, set sw, remove  */

/*990530 - change packed sign handling - signpplast, optn s1/s2, etc    */
/* - tstsignp stores signpplast, setsigns() added at end getop()        */
   unpack(w3,w2,0x01);                    /* unpack signed (ascii zones)*/

/* convert numeric string to integer long or long long depending I32/I64*/
   /* wbin = strtoll(w3,(char**)0,10); May19/11*/
   /* wbin = atol(w3); Oct04/11 change back for 32bit gcc HP Itanium */
   wbin = strtoll(w3,(char**)0,10);

   /* if negative sign '-', reverse sign in binary data */
   if (sign == '-')
     { wbin = wbin * (-1);                /* make data absolute value  */
     }
 }
/*9804 - mvn will output from w3 for intypes k,p,z,n & out typs z,c     */

/*eject*/
/*9811 - floating point single/double added                             */
/* test for floating point, 8f=double, 4f=single                        */
else if (opdt == 'f')                     /* float/double 4f/8f ?   */
 { if (oplth == 4)                        /* binary 1 byte char ?   */
     { memcpy((Uchar*)&wflt,opptr,4);     /* retrieve to float w/a  */
       wbin = wflt;                       /* convert float to int   */
     }
   else
     { memcpy((Uchar*)&wdbl,opptr,8);     /* retrieve to double w/a */
       wbin = wdbl;                       /* convert double to int  */
     }
 }
else
 { errmsg("getop typ ERR/bug ?","",carg->cmd,0,0x22);
 }

/*990530 - packed sign handling changed, signpplast, optn s1/s2, etc */
setsigns(carg);                       /* store signpp for next output    */

return(wbin);                     /* return result                   */
}

/*eject*/
/*------------------------------- putop1 -------------------------------*/
/* store op1 output from common binary int to indicated output format */
/* - op1 area may be record output, or work area                        */
/*   (op1ptr already determined by calling function)                    */
/* - output may be various types: zoned/packed/binary,ascii/ebcdic      */

void putop1(char *recd, struct CA *carg, UVi64 binarg)
{
UVi64 fill1 = 0;              /* zero int before wbin for lth >4,I32,BEM*/
UVi64 wbin;                   /* binarg stored here 0 left 0 right     */
UVi64 fill2 = 0;              /* may help if gen I32 & user spcfs >4b  */
UVi64 wbin1;                  /* binarg as + value for zoned/packed out*/
UVi32 fill3 = 0;              /* in case I32 used with 8 byte integers */
UVi32 wint;                   /* 4/8 justified for lth 4 output        */
int  ii;
int w3l2;
float wflt;                    /* conversion w/a for float & double    */
double wdbl;
short wshort;
int opdsp;
int oplth;
char  *opptr;
char opdc;
char opdt;
Uchar *wbinpi;
Uchar *wbinp = (Uchar*)&wbin;
Uchar *wintp = (Uchar*)&wint;
int w1b;        
int w4b;        

/* 1st store field ptr & lth locally to simplify coding this function*/
/*Dec19/10 - getop arg now ptr to cargs structure                */
/*         - for now assume op1, later allow any op 1-6 in cargs */
opdsp = carg->a1d;
opptr = recd + carg->a1d;
oplth = carg->a1l;
opdt = carg->a1dt;
opdc = carg->a1dc;

/* store input arg 4/8 depending on I32/I64 with zeros left & right    */
wbin = binarg;                        /* store input with 0 left&right */

/* test for sign & if neg set +/- indic for zoned/packed ONLY (not binary)*/
/* & reverse sign to positive in wbin1 for zoned/packed conversion      */
wbin1 = wbin;                         /* copy to 2nd area for pos cnvrt */
sign = '+';                           /* presume +                      */
if (wbin1 < 0)
  { sign = '-';                       /* set neg indic for zon/pac rtns */
    wbin1 = wbin1 * (-1);             /* create pos input for zon/pac   */
  }
/*NOTE wbin1 pos used only for zoned/packed, wbin for binary alrdy has sign*/

/* test for & convert to required op1, output format                    */
setcodes(opdc);                     /* setup ascii/ebcdic codes/signs */
setsigns(carg);                     /* set sign codes for output      */

/* calc begin data index in work area for output to op1 area (zon/pac)  */
w1b = 20 - oplth;              /* calc data start byte in w/a    */

/*eject*/
/* 1st test for 'binary' output required                                */
if (opdt == 'b')                         /* binary ? (1,2,or 4)       */
 { if (oplth == 1)                       /* binary 1 byte char ?      */
     { opptr[0] = (char) wbin;
     }
   else if (oplth == 2)                  /* binary short ?            */
     { wshort = (short) wbin;              /* move input to short       */
       if (opdc == 's')
         { bswap2((Uchar*)&wshort);        /* switch big/little ends    */
         }
       memcpy(opptr,(Uchar*)&wshort,2);  /* store short in out area   */
     }
   else if (oplth == 3)                  /* binary 3 bytes ?          */
     { /* 3 bytes out, dont test swap, just BEM/LEM justify left/right  */
       wint = wbin;                        /* cnvt possible 8 bytes to 4*/
       if (BEM)
         { memcpy(opptr,wintp+1,3);      /* store right hand 3 bytes  */
         }
       else
         { memcpy(opptr,wintp,3);        /* store left hand 3 bytes   */
         }
     }
   else if (oplth == 4)                  /* binary 4 bytes ?          */
     { wint = wbin;                        /* justify 4/8 into 4        */
       if (opdc == 's')
         { bswap4((Uchar*)&wint);          /* switch big/little ends    */
         }
       memcpy(opptr,(Uchar*)&wint,4);    /* store int  in out area  */
     }
/*Feb00 - allow lengths 5-8 for UW7 long long, DEC long                 */
   else
    { /* I64 - test byte swap option before BEM/LEM                    */
       if (opdc == 's')
         { bswap8(wbinp);
         }
#if (BEM)
       /* I64+BEM - right justify in output lth                         */
       wbinpi = (wbinp+8-oplth);
       memcpy(opptr,wbinpi,oplth);
#else
       /* I64+LEM - left justify in output lth                          */
       memcpy(opptr,wbinp,oplth);
#endif
     }
 }

/*eject*/
/* test op1 data type for 'zoned'                                       */
/* ifso - convert binary to ascii string via sprintf                    */
/*      - right justify in 20 byte work area in case out field inter    */
/*      - copy replacing zones with ebcdic zones if indicated           */
/*      - set proper sign as indicated by op1 typ/cod/sgn               */
/*      - copy to indicated output area (record out or work area)       */
else if (opdt == 'z' || opdt == 'c')
 { /*Feb27/11/OM/SUN - problem sprintf %lld, changed to %ld OK */
   /* sprintf(w3,"%lld",wbin1); */  
   sprintf(w3,"%ld",wbin1);     
   w3l2 = strlen(w3);                      /* SEE NOTE BELOW 950429     */
   memset(w4,'0',20);                      /* zero fill for sprintf ascii*/
   w4[20] = '\0';
   w4b = (20 - w3l2);                      /* calc start byte in w/a    */
   sncopy(&w4[w4b],w3,w3l2,1);             /* rt adjust sprintf in w/a  */
   setsignz(w1,w4,w1b,carg->a1sgn);        /* set sign via sign code & sw*/
                                           /* also inserts zones asc/ebc*/
   memcpy(opptr,&w1[w1b],oplth);           /* copy to out area   */
 }

/* test op1 data type for 'packed'                                      */
/* ifso - convert binary to ascii string via sprintf                    */
/*      - right justify in 20 byte work area in case out field inter    */
/*      - pack data & set proper sign as indicated by op1 typ/cod/sgn   */
/*      - copy to indicated output area (record out or work area)       */
else if (opdt == 'p')
 { /*Feb27/11/OM/SUN - problem sprintf %lld, changed to %ld OK */
   /* sprintf(w3,"%lld",wbin1); */  
   sprintf(w3,"%ld",wbin1);     
   w3l2 = strlen(w3);                      /* SEE NOTE BELOW 950429     */
   memset(w4,'\0',21);                     /* packed zero fill w/a      */
   w4b = (20 - w3l2);                      /* calc start byte in w/a    */
   sncopy(&w4[w4b],w3,w3l2,1);             /* rt adjust sprintf in w/a  */
   pack(w1,w4);                            /* pack w4 to w1             */
   setsignp(w1);                           /* set sign via sign code & sw*/
   memcpy(opptr,&w1[w1b],oplth);       /* copy to out area          */
 }

/*eject*/
/*9811 - test for floating point single/double 4f/8f                    */
else if (opdt == 'f')                    /* float/double 4f/8f        */
 { if (oplth == 4)                       /* binary 1 byte char ?      */
     { wflt = wbin;                        /* convert int to float      */
       memcpy(opptr,(Uchar*)&wflt,4);    /* copy float to op1 area    */
     }
   else
     { wdbl = wbin;                        /* convert int to double     */
       memcpy(opptr,(Uchar*)&wdbl,8);    /* copy double to op1 area   */
     }
 }
else
 { errmsg("putop1 typ ERR/bug ?","",carg->cmd,0,0x22);
 }
return;
}

/*950428 - problem compiling on SUN ucbcc at MTM Allan Crowther      */
/*       - improper ptr/int combination op "=" on w3l = sprintf(...) */
/*       - does SUN sprintf return a ptr vs an int ??????????????    */
/*       - I replaced the return value of sprintf with sep strlen    */

/*eject*/
/*---------------------------- setcodes -----------------------------*/
/* setup codes for digits, zones, & signs depending on ascii/ebcdic  */
/* arg1 = ascii/ebcdic code a/e (default to ascii if not e)          */
/* setup zoned/packed sign for current field depending on ascii/ebcdic*/

void setcodes(char code)
{
if (code == 'e')
  { zero = zeroebc;
    nine = nineebc;
    signzp = signebczp;
    signzn = signebczn;
    signsepn = signsepne;
    signsepp = signseppe;
  }
else
  { zero = zeroasc;
    nine = nineasc;
    signzp = signasczp;
    signzn = signasczn;
    signsepn = signsepna;
    signsepp = signseppa;
  }
return;
}

/*eject*/
/*---------------------------- setsigns ------------------------------*/
/* setup zoned/packed sign for current field depending on ascii/ebcdic*/
/* set code for packed positive sign, depending on option s0/s1/s2    */
/* on current instruction (or rop=s0/s1/s2 at program init)           */
/* default is to use last input packed positive sign (from last getop)*/
/*9906 - allow instrn optn s0/s1/s2 to override run options           */

void setsigns(struct CA *carg)
{

/*Dec26/10 - disable next 7 lines      */
/* signpp = signpplast;                */
/* if (carg->opi['s'-'a'] & 0x01)      */
/*   { signpp = signpp1;               */
/*   }                                 */
/* else if (carg->opi['s'-'a'] & 0x02) */
/*   { signpp = signpp2;               */
/*   }                                 */

/* store sign to be used on next putop1 depending on sign switch +/-  */
if (sign == '-')
  { signzone = signzn;
    signpack = signpn;
  }
else
  { signzone = signzp;
    signpack = signpp;
  }
return;
}

/*eject*/
/*---------------------------- tstsignp -----------------------------*/
/* test for sign in packed work area                                 */
/* - signcodes to test for predetermined depending on ascii/ebcdic   */
/* - do not modify signs in the data area                            */
/* - set sign indicator to '+' or '-'                                */
/* - if packed sign positive, save for next putop1 packed output     */
/*   unless overridden by run optn s1/s2 or instrn optn s1/s2        */
/*9907 - add options s16 - inhibit storing sign from this instrn op2 */
/*9907 - deflt validate C/F -> C, optn s4 -> F, s8 -> store as is    */

void tstsignp(char *wa, struct CA *carg)
{
Uchar wsign;                       /* rt half nibble isolated here   */

/* setup sign indicator  - presuming ascii positive                  */
sign = '+';

/* extract sign from rt half of units byte                           */
wsign = (Uchar) ((intU) wa[19] & 0x0f); /* isolate sign to sep byte  */

if (wsign == signpn)                 /* sign nibble = packed neg code*/
  { sign = '-';
  }

return;
}

/*eject*/
/*----------------------------- tstsignz -----------------------------*/
/* test for sign in zoned work area & set separate sign ind -/+       */
/* also copy to alt w/a removing any separate sign bytes & nonnums    */
/* & replacing zones with ascii numeric zones                         */

void tstsignz(char *out, char *in, struct CA *carg)
{
int ii,jj;
Uchar wsign;                  /* zone of units isolated here for test */
Uchar wsdig;                  /* units digit isolated here for test   */
char *pp;

sign = '+';                   /* presume sign positive                */

/* 1st test for code 'x' EBCDIC zone signs alrdy converted to ASCII   */
/* pos 0-9 {ABCDEFGHI 0x40-0x49 to 0123456789 0x30-0x39               */
/* neg 0-9 }JKLMNOPQR 0x40-0x49 to pqrstuvwxy 0x70-0x79 (MicroFocus)  */

if (carg->a1dc == 'x')
  { in[19] = EBs2MFs[ (Uchar) in[19] ]; 
  }

/* test zone of units byte to predetermined code for ascii/ebcdic +/- */
wsign = (Uchar) ((intU) in[19] & 0xf0); /* isolate zone sign to w/s */
wsdig = (Uchar) ((intU) in[19] & 0x0f); /* isolate units digit  w/s */
/* test units byte for neg sign x'40' bit if ASCII (common MF or MBP)  */
/* if EBCDIC - test entire zone nibble x'D0'                           */
if (carg->a1dc == 'e')
  { if ((wsign == signebczn) && (wsdig <= 9))
        sign = '-';
  }
else
  { if (((wsign == 0x70) || (wsign == 0x40)) && (wsdig <= 9))
       sign = '-';
  }

if (sign == '-')
  { in[19] = (char) ((intU) in[19] & 0x0f); /* remove sign from input*/
    in[19] = (char) ((intU) in[19] | zero); /* replc w ASC/EBC zone  */
  }                                       /* so it wont get dropped below*/
else if ((wsign == 0xC0) && (wsdig <= 9))
  { in[19] = (char) ((intU) in[19] & 0x0f); /* remove sign from input*/
    in[19] = (char) ((intU) in[19] | zero); /* replc w ASC/EBC zone  */
  }

/* test option i1 to ignore negative sign in zone */
if (carg->opi['i'-'a'] & 0x01)
  { sign = '+';
  }

/*eject*/
/* copy input to output rt adjusted, detecting any sep signs,         */
/* dropping any sep signs & nonnums, inserting ascii numeric zones    */
memset(out,0x30,20);                /* zero fill output area */
out[20] = '\0';
for (ii=19,jj=19; ii >= 0; ii--)
  {  if ((in[ii] == signsepn) && (!(carg->opi['i'-'a'] & 0x02))) 
       { sign = '-'; 
       }
     if ((in[ii] == signsepn) || (in[ii] == signsepp))
       { continue;                       /* drop any separate signs */
       }
     if ((Uchar) in[ii] < zero || (Uchar) in[ii] > nine)
       { continue;                          /* drop any nonnumerics */
       }
     out[jj] = (char) ((intU) in[ii] & 0x0f);   /* clear input zone  */
     out[jj] = (char) ((intU) out[jj] | 0x30);  /* insert ascii zone */
     jj--;
  }

return;
}

/*----------------------------- setsignp --------------------------*/
/* set sign in packed work area (arg1)                             */
/* - sign determined by +/- indicator already set                  */
/* sign to be inserted already stored depending on ascii/ebcdic    */

void setsignp(char *wa)
{

/* set sign in units byte (right nibble) - already predetermined   */

wa[19] = (char) ((intU) wa[19] & 0xf0);  /* clear right nibble   */
wa[19] = (char) ((intU) wa[19] | (intU) signpack); /* insert sign*/

return;
}

/*eject*/
/*----------------------------- setsignz ---------------------------*/
/* copy zoned data,inserting sign into units zone or sep lead/trail */
/* - depending on sign indicator switch                             */
/* - replace zones in output by predermined ascii/ebcdic value      */

void setsignz(char *out, char *in, int db1, char sgncd)
{
int ii,jj;

/* setup indices for copy from right to left                        */
ii = 19; jj = 19;

/* if sep sign trailing, insert +/- in rt byte of output            */
/* - but not if pos & option to show '-' signs only                 */
if ((sgncd & 0x07) == 5)
     out[jj--] = sign;
else if (((sgncd & 0x07) == 1) && (sign == '-'))
     out[jj--] = sign;

/* copy replacing zones with predermined ascii/ebcdic value          */
while (jj >= 0)
       out[jj--] = (char) (((intU) in[ii--] & 0x0f) | zero);

/* now insert sep sign leading -/+ if indicated by options           */
if ((sgncd & 0x07) == 7)
     out[db1] = sign;
else if (((sgncd & 0x07) == 3) && (sign == '-'))
     out[db1] = sign;

/* if no sep sign byte - insert sign into zone of units byte         */
if ((sgncd & 0x07) == 0)
  {  out[19] = (char) ((intU) out[19] & 0x0f);  /* clear units zone*/
     if (sign == '-')
         out[19] = out[19] | signzn;
     else
         out[19] = out[19] | signzp;
  }

return;
}

/*eject*/
/*------------------------------ pack -----------------------------*/
/* pack work area 'in' to work area 'out'                          */

void pack(char *out, char *in)
{
int ii,jj;                   /* indices for work areas in/out */
intU zz,dd;

memset(out,'\0',21);         /* init output area to all 0x00's*/

/* switch sign from zone of input rt byte with digit of output rt byte */
dd = (intU) (Uchar) in[19] >> 4;
zz = (intU) in[19] << 4;
out[19] = (char) (zz | dd);


/* init indices for loop to cnvrt each pair unpacked to 1 packed byte   */
ii = 19 - 2;                   /* init in index to 100s & 10s digits   */
jj = 19 - 1;                   /* init out index for 100s+10s same byte*/

/* begin loop to pack each pair of input bytes to 1 output until left side */
while (jj > 0)
  { dd = (intU) in[ii+1] & 0x0f;
    zz = (intU) in[ii] << 4;
    out[jj] = (char) (zz | dd);
     ii--; ii--;               /* decrmnt for next pair input bytes    */
     jj--;                     /* decrmnt to next output byte          */
  }

return;
}

/*eject*/
/*------------------------------ unpack ---------------------------*/
/* unpack from work area 'in' to work area 'out'                   */
/* input data is right adjusted in x00 filled work area            */
/* ensure all zones are ascii numeric including units zone (sign)  */
/*9803 - add op3 bit ctl for packed sign/nosign 1/0                */

void unpack(char *out, char *in, short bits)
{
int ii,jj;
intU zz,dd;

memset(out,0x30,20);         /* init out area to ascii zeros */
out[20] = '\0';

/* IF SIGNED - reverse halves of right byte of input                  */
/*           - init indices to start 2nd byte from right side         */
/*      else - init indices to start 1st byte from right side         */
if (bits & 0x01)
  {
    dd = (intU) (Uchar) in[19] >> 4; /* shift units digit to rt half */
    out[19] = (char) (0x30 | dd);      /* force zone to ascii          */
    ii = 19-1;                   /* start input 2nd byte from rt side  */
    jj = 19-2;                   /* start output at 3rd & 2nd byte pair*/
  }
else
  { ii = 19;                     /* start input 1st byte from rt side  */
    jj = 19-1;                   /* start output at 2rd & 1nd byte pair*/
  }

/* begin loop to unpack each byte to 2 output until left end output   */
while (jj > 0)                  /* could shorten to save time ???     */
   {
     dd = (intU) in[ii] & 0x0f;
     zz = (intU) (Uchar) in[ii] >> 4;
     out[jj+1] = (char) ((intU) out[jj+1] | dd);
     out[jj]   = (char) ((intU) out[jj]   | zz);
     ii--;                      /* decrmnt to next packed input byte  */
     jj--; jj--;                /* decrmnt for next pair out bytes    */
  }

return;
}

/*eject*/
/*----------------------------- bswap2 ----------------------------*/
/* convert big-end/little-end if indicated by options              */
/* arg1 - char ptr to short                                        */

short bswap2(Uchar *ptr)
{
short ws;                             /* retrieval w/s for short    */
Uchar *wptr;

/* retrieve callers short data into temp w/s                        */
wptr = (Uchar *) &ws;                 /* setup char ptr to short    */
memcpy(wptr,ptr,2);                   /* copy to w/s unchanged      */

ptr[0] = wptr[1];                     /* copy back with switch      */
ptr[1] = wptr[0];

return(1);
}

/*eject*/
/*----------------------------- bswap3b ---------------------------*/
/* switch big-end/little-end for 3 bytes binary on BEM/RISC        */
/* arg1 - char ptr to int                                        */

short bswap3b(Uchar *ptr)
{
int  wl;                             /* retrieval w/s for int    */
Uchar *wptr;

/* retrieve callers int  data into temp w/s                        */
wptr = (Uchar *) &wl;                  /* setup char ptr to int    */
memcpy(wptr,ptr,4);                    /* copy to w/s unchanged      */

/* copy back with switch for 3 bytes BEM/RISC                        */
ptr[0] = '\0';              /* 1st byte unused for 3 byte BEM switch */
ptr[1] = wptr[3];
ptr[2] = wptr[2];
ptr[3] = wptr[1];

return(1);
}

/*----------------------------- bswap3i ---------------------------*/
/* switch big-end/little-end for 3 bytes binary on INTEL           */
/* arg1 - char ptr to int                                        */

short bswap3i(Uchar *ptr)
{
int  wl;                             /* retrieval w/s for int    */
Uchar *wptr;

/* retrieve callers int  data into temp w/s                        */
wptr = (Uchar *) &wl;                  /* setup char ptr to int    */
memcpy(wptr,ptr,4);                    /* copy to w/s unchanged      */

/* copy back with switch for 3 bytes Intel                           */
ptr[0] = wptr[2];
ptr[1] = wptr[1];
ptr[2] = wptr[0];
ptr[3] = '\0';            /* 4th byte unused for 3 byte INtel switch */

return(1);
}

/*eject*/
/*----------------------------- bswap4  ---------------------------*/
/* convert big-end/little-end if indicated by options              */
/* arg1 - char ptr to int                                        */

short bswap4(Uchar *ptr)
{
int  wl;                             /* retrieval w/s for int    */
Uchar *wptr;

/* retrieve callers int  data into temp w/s                        */
wptr = (Uchar *) &wl;                  /* setup char ptr to int    */
memcpy(wptr,ptr,4);                    /* copy to w/s unchanged      */

ptr[0] = wptr[3];                      /* copy back with switch      */
ptr[1] = wptr[2];
ptr[2] = wptr[1];
ptr[3] = wptr[0];

return(1);
}

/*eject*/
/*----------------------------- bswap8  ---------------------------*/
/* convert big-end/little-end if indicated by options              */
/* arg1 - char ptr to int                                        */

short bswap8(Uchar *ptr)
{
UVi64  wll;                            /* retrieval w/s for int    */
UVi32  xx = 0;                         /* saftey                     */
Uchar *wptr;

/* retrieve callers int  data into temp w/s                        */
wptr = (Uchar *) &wll;                 /* setup char ptr to int    */
memcpy(wptr,ptr,8);                    /* copy to w/s unchanged      */

ptr[0] = wptr[7];                      /* copy back with switch      */
ptr[1] = wptr[6];
ptr[2] = wptr[5];
ptr[3] = wptr[4];
ptr[4] = wptr[3];
ptr[5] = wptr[2];
ptr[6] = wptr[1];
ptr[7] = wptr[0];

return(1);
}

/*eject*/
/*---------------------------------------------------------------------*/
/* 971107+ - following functions included in program for internet share*/
/*         - originally linked in from uvlib.a archive                 */

/*uvhd----------------------- atol1 -------------------------------*/
/* convert ascii digit string to int integer                      */
/* - also see atol3 which has multiple stop chars vs atol1 1 only  */
/*                                                                 */
/* arg1 - ptr to int where result is to be stored                 */
/*                                                                 */
/* arg2 - pointer to 1st byte of data to be converted              */
/*                                                                 */
/* arg3 - stop character (in addition to NULL if bit 0x01)         */
/*        (if not needed code any dummy nonnumeric char)           */
/*                                                                 */
/* arg4 - max stop count                                           */
/*                                                                 */
/* arg5 - bit codes to modify procedure as follows:                */
/*        0x01 - stop on null, else by stopc or max count          */
/*        0x02 - stop on any nonnumeric except '-'                 */
/*             - return result of calculation so far + or -        */
/*        0x04 - bypass the stop character in ptr returned         */
/*        0x10 - recognize '-' anywhere in input & set result neg  */
/*               (otherwise ignore '-')                            */
/*        0x20 - validate string all numeric digits or '-' signs   */
/*             - if any other char found, return -1                */
/*             - if this bit not on, nonnumerics are bypassed      */
/*                                                                 */
/* return - ptr to the stop char that ends conversion              */
/*          or 1 past the stop char if bit 0x40 present            */
/*          or the current char when max lth is reached            */
/*        - may point to the null char at end of input sring       */

char *atol1(int *result, char *in, char stop, int max, short bits)
{
int nn=0;                /* result to be returned                 */
int ll;                  /* length ctr for max lth chk            */
short sign = 1;          /* neg indicator set -1 if '-' found     */

/*eject*/
/* for loop to search for ascii digits in string & accumulate result */

for (ll=0; ll < max; ll++, in++)
  {
    if ((*in == '\0') && (bits & 0x01))     /* end null reached ?     */
         break;

    if (*in == stop)                        /* stop char reached ?    */
      { if (bits & 0x04)
            in++;                           /* bypass the stop char   */
        break;
      }

    if (*in >= '0' && *in <= '9')
        nn = 10 * nn  + (*in - '0');
    else if ((*in != '-') && (bits & 0x02))
        break;
    else if ((*in == '-') && (bits & 0x10))
      { sign = -1;
        continue;
      }
    else if (bits & 0x20)
      { nn = -1;                     /* set result -1 for invalid digit */
        break;
      }
   }

/* make negative if - found & bit 0x10 & store result in users int     */
/* - return ptr to stop char (or stop+1), max lth char,or null char     */
*result = (nn * sign);
return(in);
}

/*eject*/
/*uvhd----------------------- atol2 -------------------------------*/
/* atol2.c - convert ascii string to int integer                  */
/*  - older version, may phase out sometime                        */
/* arg1 - pointer to 1st byte of data to be converted              */
/* arg2 - alternate stop character (in addition to NULL)           */
/*        (if not needed code any dummy nonnumeric char)           */
/* arg3 - max stop count                                           */
/* arg4 - bit codes to modify procedure as follows:                */
/*        0x01 - stop on null, else by stopc or max count          */
/*        0x02 - stop on any nonnumeric except '-'                 */
/*             - return result of calculation so far + or -        */
/*        0x04 - recognize '-' anywhere in input & set result neg  */
/*               (otherwise ignore '-')                            */
/*        0x08 - validate string all numeric digits or '-' signs   */
/*             - if any other char found, return 0                 */
/*             - if this bit not on, nonnumerics are bypassed      */

int atol2(char *str, char stopc, int max, short bits)
{
int num=0;               /* result to be returned                 */
int j;                   /* work index                            */
short sign = 1;          /* neg indicator set -1 if '-' found     */

for (j=0; j < max && str[j] != stopc; j++)
   {
     if ((bits & 0x01) && (str[j] == '\0'))
          break;
     if (str[j] >= '0' && str[j] <= '9')
         num= 10 * num + (str[j] - '0');
     else if ((str[j] != '-') && (bits & 0x02))
              break;
     else if ((str[j] == '-') && (bits & 0x04))
           { sign = -1;
             continue;
           }
     else if (bits & 0x08)
             return(0);
   }
return(num * sign);
}

/*eject*/
/*uvhd----------------------- atol3 -------------------------------*/
/* convert ascii digit string to integer Uvint (32 bits)           */
/* - also see atol1 which has only 1 stop char vs multiple         */
/*                                                                 */
/* arg1 - ptr to int where result is to be stored                 */
/* arg2 - pointer to 1st byte of data to be converted              */
/* arg3 - stop characters (in addition to NULL if bit 0x01)        */
/*        (if not needed code any dummy nonnumeric char)           */
/* arg4 - max stop count                                           */
/* arg5 - bit codes to modify procedure as follows:                */
/*        0x01 - stop on null, else by stopc or max count          */
/*        0x02 - stop on any nonnumeric except '-'                 */
/*             - return result of calculation so far + or -        */
/*        0x04 - bypass the stop character in ptr returned         */
/*        0x10 - recognize '-' anywhere in input & set result neg  */
/*               (otherwise ignore '-')                            */
/*        0x20 - validate string all numeric digits or '-' signs   */
/*             - if any other char found, return -1                */
/*             - if this bit not on, nonnumerics are bypassed      */

/* return - ptr to the stop char that ends conversion              */
/*          or 1 past the stop char if bit 0x40 present            */
/*          or the current char when max lth is reached            */
/*        - may point to the null char at end of input sring       */

char *atol3(int *result, char *in, char *stops, int max, short bits)
{
int nn=0;                /* result to be returned                 */
int ll;                  /* length ctr for max lth chk            */
int jj;                  /* index for stops char search           */
short sign = 1;          /* neg indicator set -1 if '-' found     */

/*eject*/
/* for loop to search for ascii digits in string & accumulate result */

for (ll=0; ll < max; ll++, in++)
  {
    if ((bits & 0x01) && (*in == '\0'))
         break;

    for (jj=0; stops[jj] > '\0'; jj++)
         if (stops[jj] == *in)
             break;
    if (stops[jj] != '\0')
      { if (bits & 0x04)
            in++;                               /* bypass the stop char   */
        break;
      }

    if (*in >= '0' && *in <= '9')
        nn = 10 * nn  + (*in - '0');
    else if ((*in != '-') && (bits & 0x02))
        break;
    else if ((*in == '-') && (bits & 0x10))
      { sign = -1;
        continue;
      }
    else if (bits & 0x20)
      { nn = -1;                     /* set result 0 for invalid digit */
        break;
      }
   }

/* make negative if - found & bit 0x10 & store result in users outfield */
/* - return ptr to stop char (or stop+1), max lth char,or null char     */
*result = ( nn * sign);
return(in);
}

/*eject*/
/*uvhd----------------------- sncopy --------------------------*/
/* copy op2 to op1 until the op3 max count reached             */
/* or until a null is reached if op4 has a 1 bit               */
/* - null terminate the output if op4 has a 2 bit              */
/* - return the actual length copied                           */

int sncopy(
char out[],              /* output string           */
char in[],               /* input string            */
int max,                 /* no of chars to copy     */
short bits)              /* stop on null if 1 bit   */
{                        /* null terminate if 2 bit */
int i;

for (i=0; i < max; i++)
  {  if ((bits & 0x01) && (in[i] == '\0'))
         break;             /* stop on null if op4 1 bit  */
     out[i] = in[i];        /* copy the current character */
  }

if (bits & 0x02)            /* if indicated by op4 = 2      */
    out[i] = '\0';          /* null terminate output string */

return(i);
}

/*eject*/
/*uvhd----------------------- stncopy ----------------------------*/
/* copy op2 to op1 until any of the following occurs:             */
/*     1 - an input character matches the op3 stop character      */
/*     2 - the op4 max count is reached                           */
/*     3 - a null is reached in the input (may inhibit by op5)    */
/* op5 is bit coded to control the following:                     */
/*     0x01 - also stop copy on input null (default does not)     */
/*     0x02 - null terminate the output (default does not)        */
/*     0x04 - if copy is stopped by op3 char include it in output */
/* - return the actual data byte count copied                     */

int stncopy(
char *out,                /* output string                    */
char *in,                 /* input string                     */
char stopc,               /* stop character                   */
int max,                  /* max bytes to copy                */
short bits)               /* options (see above)              */
{
int i;

for (i=0; i < max; i++)
   { if ((bits & 0x01) && (in[i] == '\0'))
         break;
     if (in[i] == stopc)
         break;
     out[i] = in[i];      /* copy the current character   */
   }

if ((bits & 0x04) && (in[i] == stopc))
      out[i++] = stopc;
if (bits & 0x02)
    out[i] = '\0';

return(i);              /* return actual bytes copied   */
}

/*eject*/
/*--------------------------- stmcopy ----------------------------*/
/* copy op2 to op1 until any of the following occurs:             */
/*     1 - an input character matches any char in op3 string      */
/*     2 - the op4 max count is reached                           */
/*     3 - a null is reached in the input (may inhibit by op5)    */
/* return the actual data byte count copied                       */

/* op5 is bit coded to control the following:                     */
/*     0x01 - also stop copy on input null (default does not)     */
/*     0x02 - null terminate the output (default does not)        */
/*     0x04 - if copy is stopped by op3 char include it in output */
/*     0x08 - inhibit match on 1st byte of data                   */

/*Oct11/07 - add bits to inhibit char detect in quotes & parens   */
/*     0x10 - inhibit char detect within single quotes            */
/*     0x20 - inhibit char detect within single quotes            */
/*     0x40 - inhibit char detect within (parens)                 */

int stmcopy(
char *out,                /* output string                    */
char *in,                 /* input string                     */
char *stops,              /* stop characters                  */
int max,                  /* max bytes to copy                */
short bits)               /* options (see above)              */
{                   
int ii,jj,kk,q1,q2,pl,pr;

kk=0;q1=0;q2=0;pl=0;pr=0;

if (bits & 0x08)
  { kk = 1; 
  }

/*eject*/
/*Oct11/07 - inhibit char detect if in quotes or parens       */
/* - count special chars to inhibit detections                */
/* - when target detected, ignore/continue if counts indicate */
for (ii=0,jj=0; ii < max; ii++)
   { if (in[ii] == '\'') q1++;
     if (in[ii] == '\"') q2++;
     if (in[ii] == '(')  pl++;
     if (in[ii] == ')')  pr++;

     if ((bits & 0x01) && (in[ii] == '\0'))
       { break;
       }

     for (jj=0; stops[jj] > 0; jj++)
        { /* ignore match on 1st byte if bits 0x08 (kk set 1 above) */
          if ((stops[jj] == in[ii]) && (ii >= kk))
            { /*Oct11/07 - inhibit char detect if in quotes or parens */
              if ((bits & 0x10) && (q1 % 2 == 1)) { ; }
              else if ((bits & 0x20) && (q2 % 2 == 1)) { ; }
              else if ((bits & 0x40) && (pl != pr)) { ; }
              else { break; }
            }
        }
     if (stops[jj] != '\0')
       { break;
       }
     out[ii] = in[ii];      /* copy the current character   */
   }

if ((bits & 0x04) && (in[ii] == stops[jj]))
  { out[ii++] = stops[jj];
  }
if (bits & 0x02)
  { out[ii] = '\0';
  }

return(ii);              /* return actual bytes copied   */
}

/*eject*/
/*uvhd----------------------- sdscopy -------------------------------*/
/* copy function controlled by dsplcmnt allowing bypass & stop chars */
/*                                                                   */
/* arg1 - the output area (should be as big as the input area)       */
/* arg2 - the input data string                                      */
/* arg3 - max length of input area                                   */
/* arg4 - pointer/displacement to current position in input area     */
/*      - updated to the stop char position or end op2 if nofind     */
/* arg5 - bypass or stop character string (always 4 chars)           */
/*      - 1,2 are bypass chars active if bit code 0x0300 present     */
/*      - 3,4 are stop chars always active, post bypass if 0x0030    */
/* arg6 - bit control codes                                          */
/*        0x1000 - init the displacement before this copy begins     */
/*        0x2000 - inhibit testing stop chars within single quotes   */
/*        0x0100 - bypass char#1 if present, then char#2 if present  */
/*        0x0200 - bypass multiples of #1, then multiples of #2      */
/*        0x0010 - if stopped by char #3, bypass it in dsplcmt update*/
/*               - then check for char #4,& bypass if present        */
/*        0x0020 - if stopped by char #3, bypass multiples to non #3 */
/*               - then bypass any multiple occurrences of char #4   */
/*        0x0002 - null terminate the output string                  */
/*        0x0001 - if stop search on input string null               */
/*                 dont set dsplcmnt to max                          */
/*                 (default sets dsplcmnt to max on nofind stopchar) */
/*                                                                   */
/* return - the length of the data copied                            */
/*        - note that arg4 dsplcmnt is also updated                  */
/*        - stop char nofinds, set the dsplcmnt to the max (arg3)    */

int sdscopy(char *out,char *in,int max,int *sds,char *stop,short bits)
{
int ii;
int sdi;            /* dsplcmnt retrvd to local, will update at end */
int qq=0;           /* quote counter (to inhibit stop within)       */

/* retrieve the dsplcmnt to a local var & init if bit 0x1000         */
sdi = *sds;                    /* retrieve current dsplcmnt to local */
if (bits & 0x1000)
    sdi = 0;

/* test bit controls for pre-copy bypass option single or multiple   */
if (bits & 0x0300)
  { while ((in[sdi] == stop[0]) && (sdi < max))
      { sdi++;
        if ((bits & 0x0200) == 0)       /* multi bypass bit ?        */
           break;
      }
    /* test pre-copy bypass for char #2 single or multiple           */
    while ((in[sdi] == stop[1]) && (sdi < max))
      { sdi++;
        if ((bits & 0x0200) == 0)       /* multi bypass bit ?        */
           break;
      }
  }

/*eject*/
/* copy input to output until NULL, max lth, or either stop char found*/
ii = 0;
while (sdi < max)
  { if (in[sdi] == '\'')
        qq++;                           /* count qts for stop inhibit */
    if ((bits & 0x2000) && (qq % 2))
      { ;
      }
    else
      { if (in[sdi] == stop[2])
           break;
        if (in[sdi] == stop[3])
           break;
      }
    if (in[sdi] == '\0')                /* null reached, no stop char   */
      { if ((bits & 0x0001) == 0)       /* stop null & dont set dsp max */
          { sdi = max;                  /* yes - set dsp max for nofind */
          }
        break;
      }
    out[ii++] = in[sdi++];              /* copy current byte, up to next*/
  }

/* bypass stop chars single or multiple depending on bit ctls          */
if (bits & 0x0030)                     /* bypass stop char requested ? */
  { while ((in[sdi] == stop[2] && sdi < max))
      { sdi++;
        if ((bits & 0x0020) == 0)
           break;
      }
    while ((in[sdi] == stop[3] && sdi < max))
      { sdi++;
        if ((bits & 0x0020) == 0)
           break;
      }
  }

/* null terminate the output if bit 0x0002                            */
if (bits & 0x0002)
   out[ii] = '\0';

/* store the new displacement & return lth of data copied            */
*sds = sdi;
return(ii);
}

/*eject*/
/*uvhd-------------------------- searchcm -----------------------------*/
/* search a string for 1st character that matches any character        */
/* within a 2nd string                                                 */
/* - return the index of the found char within the string              */
/*   or return -1 if nomatch found                                     */
/* arg1 - points to the string to be searched                          */
/* arg2 - points to the string of characters to be searched for        */
/* arg3 - max length of string 1 to search                             */
/* arg4 - max length of string 2 to search                             */
/* arg5 - bit codes                                                    */
/*        1 - stop on null in string 1 (else on max length)            */
/*        2 - stop on null in string 2 (else on max length)            */

int searchcm(char *s1, char *s2, int max1, int max2, short bits)
{
int i,j;

for (i=0; i < max1; i++)
  {  if ((bits & 0x01) && (s1[i] == '\0'))
         break;
     for (j=0; j < max2; j++)
       { if ((bits & 0x02) && (s2[j] == '\0'))
            return (-1);
         if (s2[j] == s1[i])
            return(i);
       }
  }
return(-1);
}

/*eject*/
/*uvhd------------------------ errmsg.c ----------------------------*/
/* errmsg.c - display error message on stderr                       */
/* arg1 = msg text, may include \n if arg2 to be on 2nd line        */
/* arg2 = addtnl info such as filename or error data string         */
/* arg3 = addtnl info#2 (null if not needed)                        */
/*      - by convention msg2=logical & msg3=physical filename       */
/* arg4 = integer to be converted to ascii numeric                  */
/*        to replace underscores '_'s in the arg1 string (if any)   */
/*        or column# for * error marker to be displayed on next line*/
/*        if bitcode 0x04 present                                   */
/* arg5 = bitcodes to modify behaviour as follows:                  */
/*        0x000_ - info msg, display no prefix, return after msg    */
/*        0x001_ - display 'ERR -' prefix, return after msg         */
/*        0x002_ - display 'FATAL ERR -' prefix, exit program       */
/*        0x004_ - display 'SYSTEM ERR -' prefix, exit program      */
/*        0x00_1 - insert LF after msg1                             */
/*        0x00_2 - insert LF after msg2                             */
/*               - will always insert LF after msg3                 */
/*        0x00_4 - use arg4 as col# for '*' marker on next line     */
/*        0x0100 - wait for operator reply before returning         */

void errmsg(char *msg1, char *msg2, char *msg3, int num, short bits)
{
char prefix[16];
char msga[80];                   /* arg1 copied here for insert of num*/
char msgb[80];                   /* arg2,3 copied to ensure null term */
char msgc[80];
char numasc[8];                  /* arg4 converted to ascii numeric */
char lf1[2] = "\n";              /* LF or blank insert after msg1   */
char lf2[2] = "\n";              /* LF or blank insert after msg2   */
int i,j;

/* 1st setup prefix depending on severity bit code                  */
prefix[0] = '\0';               /* presume info, no prefix desired */
if (bits & 0x10)
    strcpy(prefix,"ERR -");
if (bits & 0x20)
    strcpy(prefix,"FATAL ERR -");
if (bits & 0x40)
    strcpy(prefix,"SYSTEM ERR -");

/*eject*/
/* setup LFs/blanks for insert before msg2/msg3   depending on bits*/
lf1[0] = '\0';                    /* presume blank                */
lf2[0] = '\0';
if (bits & 0x01)
    lf1[0] = '\n';
if (bits & 0x02)
    lf2[0] = '\n';

/* copy users strings to w/s & ensure null terminated             */
memset(msga,'\0',80);
memset(msgb,'\0',80);
memset(msgc,'\0',80);
sncopy(msga,msg1,79,1);
sncopy(msgb,msg2,79,1);
sncopy(msgc,msg3,79,1);

/* convert arg4 integer to ascii numeric to replace any '_'s in arg1*/
sprintf(numasc,"%6d",num);
for (i=79,j=5; i >= 0; i--)
    if (msga[i] == '_')
        msga[i] = numasc[j--];

/* now display the constructed message                            */
printf("%s %s %s %s %s %s \n",prefix,msga,lf1,msgb,lf2,msgc);

/* display error marker line if bit 0x04 & arg4 col# > 0 & < 80   */
if ((bits & 0x04) && (num > 0) && (num < 78))
  { memset(msga,' ',78);           /* fill flagline with blanks   */
    msga[78] = '\n';               /* line feed at col 79         */
    msga[79] = '\0';               /* null term at col 80         */
    msga[num] = '*';               /* insert '*' at flag column#  */
    printf("%s",msga);
  }

/*eject*/
/* if syserr bit 0x40 & system errno is valid - display errno alpha */
/* if ((bits & 0x40) && (errno > 0) && (errno < sys_nerr))          */
/*     printf("%d %s\n",errno,sys_errlist[errno]);                  */
/* above replaced by perror - July 93 (undefs on svr5r4)            */
if (bits & 0x40)
    perror("FATAL ERR ");

/* return to caller if severity bits 0 or 1, exit program if 2/6    */
if (bits & 0x60)
    exit(99);
else
  { /* wait for oprtr reply if bit 0x100                            */
    if (bits & 0x100)
      { printf(" - reply to continue (or kill job) \n");
        fgets(msgc,80,stdin);
      }
  }
return;
}

/*eject*/
/*uvhd----------------------- showhelp ------------------------------*/
/* showhelp.c - display "help" screen - by O Townsend aug 92         */
/* - arg1 is ptr to array of char ptrs to strings                    */
/* - strings will be displayed on stderr until '!' is found          */
/*   in 1st char of any string or a null pointer is reached          */
/*Jan18/06 - OR until max lines displayed                            */
/*         - if bits 0x01, display blank lines up to max lines       */

int showhelp(char *msgs[], int max, short bits)
{
int ii,nn;
nn = 0;
for (ii=0; ii < max; ii++)
  { if ((!msgs[ii]) || (msgs[ii][0] == '!') || (msgs[ii][0] == '\0'))
      { if (bits & 0x01)
          { while (ii < max)
              { printf(" \n");
                ii++;
              }
          }
        break;
      }
    else
      { printf("%s\n",msgs[ii]);
      }
  }
return(ii);
}

/*eject*/
/*uvhd----------------------- waitreply1 ------------------------------*/
/* wait for oprtr reply, allowing q to quit                            */
/* arg2 - determines action if oprtr enters q=quit                     */
/*      - 0x00 to return or 0x01 to exit program                       */
/*      - if quit, write history file if not already written           */
/*Oct31/06 - name chgd to 'waitreply1' since 'waitreply' is in showhelp*/
/*         - and does not include the writehist() below                */

int waitreply1(char *reply, short quitctl)
{
fgets(reply,80,stdin);      /* wait for operator reply         */
if ((reply[0] == 'q') && (reply[1] < '0'))
  { if (quitctl & 0x01)
      { /* if (histw < 1)   */
         /*  { writehist(); */
          /* }              */
        exit(2);
      }
    else
      { return(2);
      }
  }
return(1);
}

/*eject*/
/*------------- convert unix time to date/time string------------*/
/* arg2 points to a unix time (secs since 1970)                  */
/* converts unix time to "ccyymmddhhmmss" & stores in arg1       */
/* arg1 = ptr to 14 byte char array to receive "ccyymmddhhmmss"  */
/*        (not null terminated)                                  */

void cnvdttm(char *dtp, time_t unixtm)
{
struct tm *p;
int month;
char dtws[16];

p = localtime(&unixtm);
month = p->tm_mon + 1;

sprintf(dtws,"%04d%02d%02d%02d%02d%02d",
p->tm_year+1900,month,p->tm_mday,
p->tm_hour,p->tm_min,p->tm_sec);

memcpy(dtp,dtws,14);

return;
}

/*eject*/
/*uvhd----------------------- hex2data -------------------------------*/
/* hex2data.c - function to convert hex representation to true data   */
/*            - by Owen Townsend, ARC, AUG92, updt NOV92              */
/* - returns count of output data length converted                    */
/* arg1 - points to output area  (input data will be twice as int)   */
/* arg2 - points to input hex rep (either upper or lower case a-f)    */
/* arg3 - optional stop char (if not needed code an unlikely char)    */
/* arg4 - max length of output (stop if reached)                      */
/* arg5 - bit options                                                 */
/*        0x01 - stop if null reached in input                        */
/*        0x02 - null terminate the output string                     */

int hex2data(char *out, char *in, char stopc, int max, short bits)
{
int i,j;
intU z,d;                              /* work areas for zone & digit */

for (i=0,j=0; j < max; i++,i++,j++)
 {  if ((bits & 0x01) && (in[i] == '\0'))
        break;
    /*Mar25/14 - bug if stopc odd byte, shift to next byte & clear current*/
    if (in[i+1] == stopc)       /* 2nd byte of pair should not be stopc */
      { in[i+2] = stopc;        /* correct position of ending stopc */
        in[i+1] = '\0';         /* set 2nd byte of pair 0x00        */
      }
    if (in[i] == stopc)
        break;

   z = (intU)(in[i] & 0x7f);          /* remove any high bit of zone rep  */
   if (z > 'Z') z = z & 0x5f;         /* if upper, convert to lower       */
   if (z > '9') z = z -7;             /* if a-f convert low 4 bits to a-f */
                                      /* 0x41:0xf4 - 7 = 0x3a:0x3f        */
   z = z << 4;                        /* shift result to zone & clr digit */

   d = (intU)(in[i+1] & 0x7f);        /* remove any high bit of digit rep */
   if (d > 'Z') d = d & 0x5f;         /* if upper, convert to lower       */
   if (d > '9') d = d -7;             /* if a-f convert low 4 bits to a-f */
                                      /* 0x41:0xf4 - 7 = 0x3a:0x3f        */
   d = d & 0x0f;                      /* clear zone bits                  */

   out[j] = (char)(z | d);            /* combine zone & digit into output */
 }

if (bits & 0x02)                      /* option to null term output ?     */
    out[j] = '\0';

return(j);                            /* return the data length converted */
}

/*eject*/
/*uvhd------------------------ sortops ---------------------------------*/
/* sortops.c - process options into alpha position                      */
/*               in character,& integer arrays                          */
/* - this subrtn is commonly used to process an option string from the  */
/*   command line into sorted forms easily accessed by the user programs*/
/* - options must be contiguous as in following examples:               */
/*                                                                      */
/*  abc                 - no spaces between option letters              */
/*  cba                 - not necessarily in sequence                   */
/*  b512e512r256        - option letters may have numeric args          */
/*  b512,e512,r256      - may use , separators if desired               */
/*                                                                      */
/* arg1 - input string                                                  */
/* arg2 - output sorted array of characters (or Nulls)                  */
/* arg3 - output sorted array of int ints                              */
/* arg4 - control bits - 0x00 arrays will be cleared to nulls           */
/*        0x01 - arrays will not be cleared (any prior optns remain)    */
/*        0x10 - return success/fail (+1/-1) indication to caller       */
/*               rather than errmsg&exit if invalid input detected      */

int sortops(char *opsu, char *opsc, int *opsi, short bits)
{
int ui;                    /* index to the input raw option string    */
char c;                    /* current option letter being processed   */
int ci;                    /* index to the character array            */
int j;                     /* misc index                              */
char nums[16];             /* numerics following current optn ltr     */
int ni;                    /* index to numeric array                  */

/* 1st clear the output arrays - unless inhibited by bits 0x01         */
if ((bits & 0x01) == 0)
  {  for (j=0; j < 26; j++)
        { opsc[j] = 0;             /* init the character array to nulls */
          opsi[j] = 0;             /* init integer array to zeros       */
        }
  }

/* bypass any leading '-' option identifier                             */
ui = 0;                        /* init index to 1st char                */
if (opsu[ui] == '-')
    ui++;                      /* bypass any leading '-'                */

/*eject*/
/* begin loop to extract each optn letter into opsc[] alpha sorted array*/
/* & to convert any appended digits to binary & store in opsi[] array   */
while (opsu[ui] > ' ')         /* until end string null/blank ?         */
  { c = opsu[ui++];            /* save current option letter            */
    if (c < 'a' || c > 'z')
       { printf("sortops invalid at byte# %d %s \n",ui,opsu);
         if (bits & 0x10)      /* return fail code or exit here ?       */
             return(-1);
         else
             exit(-1);
       }
    ci = c - 'a';              /* index into sorted options array       */
    opsc[ci] = c;              /* store optn letter into sorted array   */

    /* now isolate optional numeric string following alpha letter       */
    /*  convert to binary & store in integer array                      */
    ni = 0;
    while (opsu[ui] >= '0' && opsu[ui] <= '9')
           nums[ni++] = opsu[ui++];      /* store current digit        */
    nums[ni] = '\0';                     /* null terminate num string  */
    opsi[ci] = atol2(nums,'~',16,3);     /* cnvrt numstring to integer */

    /* bypass any comma ',' separator before next alpha option letter  */
    if (opsu[ui] == ',')
        ui++;
  }
return(1);
}

/*eject*/
/*uvhd------------------------ sortops4 --------------------------------*/
/* sortops4.c - process options into alpha position                     */
/*               in character, integer,& string arrays                  */
/* This subrtn is commonly used to process an option string from the    */
/* command line into sorted forms easily accessed by the user programs  */
/* Options must be contiguous as in following examples:                 */
/*                                                                      */
/*  option input string - example description                           */
/*  ------------------------------------------------------              */
/*  abc                 - no spaces between option letters              */
/*  cba                 - not necessarily in sequence                   */
/*  b512e512-r256       - option letters may have numeric args&trlng sign*/
/*  b512,e512,r256      - may use , separators if desired               */
/*  b=512,e=512,r=256   - may use = separators if desired               */
/*                        (must use if any alpha in args)               */
/*  a=jkl,b=xyz         - must use = sep to assign non-numeric strings  */
/*  a='cat',b='c t'     - may enclose value in single quotes            */
/*                        (must do if any embedded blanks)              */
/*  a=^01ef^            - indicates hexadecimal string                  */
/*                      - converted to true hex when stored             */
/*                 note - must enclose hex strigs in '^'s (circumflexes)*/
/*                        because quotes are removed by the shell       */
/*                                                                      */
/* bit control arg#6                                                    */
/*     0x10 - return success/fail (+1/-1) indication to caller          */
/*            rather than exit here if invalid input detected           */
/*                                                                      */
/* note - also see sortops.c which provides only the char sorted array  */
/*        & the integer array, but not the string ptrs & string buffer  */
/*      - used by programs not requiring the strings (less overhead)    */

int sortops4(char*opsu,char*opsc,int*opsi,char**opsp,char*opsb,short bits)
{
/*       opsu[];           pointer to unsorted raw options input      */
/*       opsc[];           ptr to 26 sorted character options         */
/*       opsi[];           ptr to 26 integer options                  */
/*       *opsp[];          ptr to 26 ptrs to option strings           */
/*       opsb[];           buffer for option strings                  */

char *opsbp;               /* ptr to current position in buffer        */
int i;                     /* index to the input raw option string     */
char c;                    /* current option letter being processed    */
int cp;                    /* index to the character array             */
int j,k;                   /* work indexes                             */
char temp[128];            /* temp w/s if hex a=^hexchars^             */

/*eject*/
/* 1st init the output arrays */
for (j=0; j < 26; j++)
   { opsc[j] = 0;             /* init the sorted result to blanks      */
     opsp[j] = (char *) 0;    /* init string ptrs to null              */
     opsi[j] = 0;             /* init integer array to zeros           */
   }
opsbp = opsb;                 /* init buffer ptr to 1st byte           */

/* bypass any leading '-' option identifier                            */
i = 0;                         /* init index to 1st char                */
if (opsu[i] == '-') i++;       /* bypass any leading '-'                */

/* begin loop to extract optn letters into opsc[] alpha sorted array   */
/*  & store any following optn prmtr/args into opsb[] array            */
while (opsu[i] > ' ')          /* until end string null/blank ?        */
  { c = opsu[i++];             /* save current option letter            */
    if (c < 'a' || c > 'z')
       { printf("sortops4 invalid at byte# %d %s \n",i,opsu);
         if (bits & 0x10)
             return(-1);
         else
             exit(-1);
       }
    cp = c - 'a';              /* index into sorted options array         */
    opsc[cp] = c;              /* store optn letter into sorted array     */

/* 1st store the data following the option letter into the string buffer*/
/* (will then convert any numeric digits from strings into integer optns*/
/*  following data of 2 types - numeric or nonnumeric                   */
/* numeric data - stored until nonnum or non '-' sign reached           */
/* nonnum data  - must have begin with '=' sep & end with ',' sep       */
/* - may be enclosed in quotes for char 'xx x' or circs for hex ^xxxx^  */
  if (((opsu[i] >= '0') && (opsu[i] <= '9')) || (opsu[i] == '-'))
    { opsp[c-'a'] = opsbp;               /* store ptr to 1st digit      */
      while (((opsu[i] >= '0') && (opsu[i] <= '9')) || (opsu[i] == '-'))
             *opsbp++ = opsu[i++];        /* store numeric digits       */
      *opsbp++ = (char) 0;               /* null terminate              */
    }

/*eject*/
/* non-num follows optn ltr - test for '=' sep & char string            */
  else if (opsu[i] == '=' && opsu[i+1] != '^')  /* =charstring ?         */
    { i++;                              /* bypass the '=' separator     */
      if (opsu[i] == '\'')              /* if enclosed in quotes ?      */
          i++;                          /* bypass opening quote         */
      opsp[c-'a'] = opsbp;              /* store ptr to 1st char string */
      while ( opsu[i] != ',' && opsu[i] != '\0')
          *opsbp++ = opsu[i++];          /* store a/n string char       */
      *opsbp++ = '\0';                  /* null terminate               */
   }

/* non-num follows optn ltr - test for '=^' sep & hex string             */
  else if (opsu[i] == '=' && opsu[i+1] == '^')  /* =charstring ?         */
    { opsp[c-'a'] = opsbp;              /* store ptr to 1st char string */
      i++; i++;                         /* bypass the leading '=^' sep  */
      j = 0;                            /* init index for temp w/s copy */
      while ( opsu[i] != ','&& opsu[i] != '^' && opsu[i] != '\0')
          temp[j++] = opsu[i++];        /* copy hex chars to temp w/s   */
      temp[j] = '\0';                   /* null terminate temp w/s      */
      k = hex2data(opsbp,temp,'~',64,1);
                                        /* cnvrt to true hex in buffer  */
      opsbp = opsbp + k;                /* incrmnt buf ptr by hex lth   */
      *opsbp++ = '\0';                  /* null term hex data in buffer */
   }

  /* end current option letter - bypass any trailing separators b4 next */
   while (opsu[i] == ','|| opsu[i] == '\'' || opsu[i] == '^')
          i++;                         /* bypass , & ^ seps b4 next op  */
 }
/* we have completed extraction of optn characters & strings            */
/* - now convert any numeric strings to integers                        */
for (k=0; k < 26; k++)
   if ((opsp[k]) && (opsp[k][0] >= '0') && (opsp[k][0] <= '9'))
       opsi[k] = atol2(opsp[k],'~',16,0x07);
return(1);
}

/*eject*/
/*uvhd-------------------- toascii2 ---------------------------*/
/* convert a string from EBCDIC to ASCII                       */
/* arg1 - string to be translated                              */
/* arg2 - max length of string                                 */
/* arg3 - bit control codes                                    */
/*        0x01 - stop on NULL                                  */
/*        0x02 - terminate output string (if max with no null) */
/*        0x04 - convert non-printable EBCDIC chars to periods */
/* return -  length of string (or max count)                   */

/*Apr04/06 - changed 0x4B (EBCDIC period) to 0x5F (EBCDIC carat) */
/*Apr04/06 - dont like it change it back to period               */

int toascii2(unsigned char *str, int max, short bits )
{
int ii;
for (ii=0; ii < max; ii++)
  { if ((str[ii] == '\0') & (bits & 0x01))
        break;
    if (((unsigned char)str[ii] < 0x40) && (bits & 0x04))
        str[ii] = 0x4B;    /*Apr04/06 change 0x4B to 0x5F & back */
    str[ii] = ebc2asc[(unsigned char)(str[ii])];
  }
if ((ii == max) && (bits & 0x02))
    str[max] = '\0';
return(ii);                            /* return lth of string  */
}

/*uvhd-------------------- toebcdic2 ---------------------------*/
/* convert a string from ASCII to EBCDIC                       */
/* arg1 - string to be translated                              */
/* arg2 - max length of string                                 */
/* arg3 - bit control codes                                    */
/*        0x01 - stop on NULL                                  */
/*        0x02 - terminate output string (if max with no null) */
/* return -  length of string (or max count)                   */

int toebcdic2(unsigned char *str, int max, short bits )
{
int ii;
for (ii=0; ii < max; ii++)
  { if ((str[ii] == '\0') & (bits & 0x01))
        break;
    str[ii] = asc2ebc[(unsigned char)(str[ii])];
  }
if ((ii == max) && (bits & 0x02))
    str[max] = '\0';
return(ii);                            /* return lth of string  */
}

/*eject*/
/*uvhd--------------------- getline -----------------------------*/
/* get the next record from a file                               */
/* return the record size read or -1 if EOF or I/O error occurs  */
/* - return lth will be 1 higher than data lth if stopchar nofind*/
/*                                                               */
/* arg1 - the file pointer                                       */
/* arg2 - points to the 1st byte of data to be written           */
/* arg2 - the count to be written, or max if stops not found     */
/* arg4 - the stop character (usually a LineFeed)                */
/* arg5 - bitcodes to control procedures as follows              */
/*                                                               */
/*default - read the count specified (ignore NULLs, LFs,CRs)     */
/*        - dont convert any chars to blanks or NULLs            */
/*        - dont null terminate                                  */
/*                                                               */
/* stop read options - dflt stop on LF                           */
/* 0x0010 - bypass any LF's (use when stopchar is CR)            */
/* 0x0020 - stop reading when the stopchar (arg4) detected       */
/* 0x0040 - stop reading when a NULL is encountered              */
/* 0x0080 - n/u                                                  */
/*                                                               */
/* 0x0001 - convert CR's to blanks                               */
/* 0x0002 - convert LF's to blanks                               */
/* 0x0004 - convert NULL's to blanks                             */
/* 0x0008 - convert tabs to blanks (anywhere in record)          */
/*                                                               */
/* 0x0100 - remove any trailing blanks                           */
/* 0x0200 - terminate record with the stopchar (arg4)            */
/*          (stops read but is not put in data automatically)    */
/* 0x0400 - null terminate the record before returning           */
/*        - would follow the stopchar if 0x0200 on               */


int getlin0(FILE *fp, char *rec, int count, int stop, short bits)
{
int cc,ii,nn;

/*eject*/
/* stop read when max count reached if no stop chars spcfd (CR,LF,NULL) */
/*  if any stop char spcfd, read 1 extra byte                           */
/*  - if extra byte stop char - ok just return count requested          */
/*  - if extra byte not stop char return length count+1 to signal error */

ii = 0; nn = 0;
while (ii <= count)
   { cc = fgetc(fp);
     if (cc == EOF)
        return(-1);
     nn++;

     if ((bits & 0x0010) && (cc == '\n'))
         continue;

     if ((bits & 0x0020) && (cc == stop))
         break;
     if ((bits & 0x0040) && (cc == '\0'))
         break;

     if (((bits & 0x0060) == 0) && (nn == count))
         break;

     if ((bits & 0x0001) && (cc == '\r'))
         cc = ' ';
     if ((bits & 0x0002) && (cc == '\n'))
         cc = ' ';
     if ((bits & 0x0004) && (cc == '\0'))
         cc = ' ';
     if ((bits & 0x0008) && (cc == '\t'))
         cc = ' ';

     rec[ii++] = (char) cc;
   }

/*eject*/
/* remove trailing blanks or 0x00's if bitcode present                */
if (bits & 0x0100)
  { for (ii--; ii > 0; ii--)
      { if ((rec[ii] != ' ') && (rec[ii] != '\0'))
             break;
      }
    ii++;                    /* correct ii to byte after last data*/
  }

/* ensure we return at least 1 blank (EOF already checked above)   */
/* - converts lines with LF only to 1 blank (& LF maybe on output) */
/* - but only if stop-char is an LF                                */
/*   to allow copying binary files with alt stop chars             */
if ((ii == 0) && (stop == '\n'))
  { rec[ii++] = ' ';
  }

/* terminate with stopchar if indicated by bitcodes                */
if (bits & 0x0200)
    rec[ii++] = stop;

/* null terminate if indicated by bitcodes                         */
if (bits & 0x0400)
    rec[ii] = '\0';

return ii;
}

/*eject*/
/*uvhd---------------------- putline ------------------------------*/
/* write a  record to a file                                       */
/* return the record size written or 0 if I/O error occurs         */
/*                                                                 */
/* arg1 - the file pointer                                         */
/* arg2 - points to the 1st byte of data to be written             */
/* arg3 - the count to be written or max if LF or null determined  */
/* arg4 - the stop-write-character (usually a LineFeed)            */
/* arg5 - bitcodes to control procedure as follows:                */
/*                                                                 */
/*default - write spcfd number of bytes (ignore NULLs,CRs,& LFs)   */
/*        - termination controlled by bitcodes & arg4 stop-char    */
/*                                                                 */
/* 0x0100 - remove any trailing blanks in the record (calc new rcsz)*/
/*                                                                 */
/* 0x0020 - stop writing on stop-character (arg4)                  */
/* 0x0040 - stop writing on NULL                                   */
/*                                                                 */
/*        - termination after max count written                    */
/* 0x0001 - write a CR after last data byte                        */
/* 0x0002 - write the stop-char (follows CR if 0x0001)             */
/* 0x0004 - write a NULL (follows CR & stopchar if indicated)      */

int putlin0(FILE *fp, char *rec, int count, int stop, short bits)
{
int ii,ll;

ll = count;                         /* presume count determines EOR */

/* 1st test for option to remove trailing blanks & adjust max count */
if (bits & 0x0100)
  { for (ll=count-1; ll > 0; ll--)
      { if (rec[ll] > ' ')
          { ll++;               /* include the last nonblank in count*/
            break;
          }
      }
    /* correct out lth for NULL records & force output to 1 blank    */
    /* - but only if stop-char is LineFeed                           */
    if ((ll == 0) && (stop == '\n'))
      { if (rec[ll] == '\0')
            rec[ll] = ' ';
        ll++;
      }
  }

/*eject*/
/* now write characters until any of following occurs:               */
/* - 1st CR   reached  & control bit present                         */
/* - 1st LF   reached  & control bit present                         */
/* - 1st NULL reached  & control bit present                         */
/* - max count (or last nonblank count) reached                      */
ii = 0;
while (ii < ll)
  { if ((bits & 0x0020) && (rec[ii] == stop))
        break;
    if ((bits & 0x0040) && (rec[ii] == '\0'))
        break;
    if (fputc(rec[ii++],fp) < 0)
        return(-1);                  /* return -1 if write err occurs*/
   }

/* append CR if indicated by bitcodes                               */
if (bits & 0x0001)
  { if (fputc('\r',fp) < 0)
        return(-1);                   /* return -1 if write err occurs*/
    ii++;
  }

/* append stop-char if indicated by bitcodes                        */
if (bits & 0x0002)
  { if (fputc(stop,fp) < 0)
        return(-1);                   /* return -1 if write err occurs*/
    ii++;
  }

/* append NULL if indicated by bitcodes                               */
if (bits & 0x0004)
  { if (fputc('\0',fp) < 0)
        return(-1);                   /* return -1 if write err occurs*/
    ii++;
  }

return(ii);                            /* return length written       */
}

/*eject*/
/*uvhd-------------------------- repchars -----------------------------*/
/* replace characters with alternates                                  */

short repchars(char *string, char *srcs, char *reps, int max, short bits)
{
int ii,jj,nn;

nn = 0;        /* clear counter of replacements for return value */

for (ii=0; (ii < max) && (string[ii]); ii++)
  { for (jj=0; srcs[jj]; jj++)
      { if (string[ii] == srcs[jj])
          { string[ii] = reps[jj];
            nn++;
            break;
          }
      }
  }
return(nn);
}

/*uvhd--------------------- getdatetime --------------------------*/
int getdatetime(void)
{
/* get system date & time & convert to "yyyymmdd_hhmm" format     */
tmsecs = time(0);              /* get time in secs since 1970     */
cnvdttm(today,tmsecs);         /* convert to "yyyymmddhhmm"       */
/* today[12] = '\0';   |* drop seconds  <-- disabled Feb18/07     */
sncopy(date6,today+2,6,0x03);  /* create today in 6 digits yymmdd */
sncopy(time6,today+8,6,0x03);  /* create time in 6 digits hhmmss  */
memcpy(todttm,today,8);           /* copy for 'yyyymmdd:hhmm' edit    */
todttm[8] = ':';                  /* insert ':' after yymmdd          */
memcpy(todttm+9,today+8,4);       /* append HHMM                      */

return(1);
}

/*eject*/
/*uvhd------------------- tolower2 ----------------------------*/
/* convert a string to lower case                              */
/* arg1 - string to be translated                              */
/* arg2 - max length of string                                 */
/* arg3 - bit control codes                                    */
/*        0x01 - stop on NULL                                  */
/*        0x02 - terminate output string (if max with no null) */
/* return -  length of string (or max count)                   */

int tolower2(char *str, int max, short bits )
{
int ii;
for (ii=0; ii < max; ii++)
  { if ((str[ii] == '\0') & (bits & 0x01))
        break;
    str[ii] = (char) tolower(str[ii]);  /* translate to lower  */
  }
if ((ii == max) && (bits & 0x02))
    str[max] = '\0';
return(ii);                             /* return lth of string*/
}

/*uvhd-------------------- toupper2 ---------------------------*/
/* convert a string to upper case                              */
/* arg1 - string to be translated                              */
/* arg2 - max length of string                                 */
/* arg3 - bit control codes                                    */
/*        0x01 - stop on NULL                                  */
/*        0x02 - terminate output string (if max with no null) */
/* return -  length of string (or max count)                   */

int toupper2(char *str, int max, short bits )
{
int ii;
for (ii=0; ii < max; ii++)
  { if ((str[ii] == '\0') & (bits & 0x01))
        break;
    str[ii] = (char) toupper(str[ii]); /* translate to lower   */
  }
if ((ii == max) && (bits & 0x02))
    str[max] = '\0';
return(ii);                           /* return lth of string  */
}

/*eject*/
/*uvhd--------------------- toprint ---------------------------*/
/* convert a string to printable characters                    */
/* - replace any unprintable characters with '.' periods       */
/* arg1 - string to be translated                              */
/* arg2 - max length of string                                 */
/* arg3 - bit control codes                                    */
/*        0x01 - stop on NULL                                  */
/*        0x02 - terminate output string (if max with no null) */
/*        0x04 - convert non-numeric chars to periods          */
/* return -  length of string (or max count)                   */

int toprint2(Uchar *str, int max, short bits)
{
int ii;
for (ii=0; ii < max; ii++)
  { if ((str[ii] == '\0') & (bits & 0x01))
      { break;
      }
    if (!(isdigit(str[ii])) && (bits & 0x04))
      { str[ii] = '.';
      }
    str[ii] = trtprint[(unsigned char)(str[ii])];  
  }
if ((ii == max) && (bits & 0x02))
  { str[max] = '\0';
  }
return(ii);                            /* return lth of string  */
}

/*eject*/
/*--------------------------- data2vhx -------------------------------*/
/* data2vhx.c - convert data to vertical hex representation           */
/*            - by Owen Townsend - ARC - aug 92                       */
/*  arg1 = output area for char data with unprntbl cnvrtd to periods  */
/*  arg2 = output data area for zones representation                  */
/*  arg3 = output data area for digits representation                 */
/*  arg4 = input data (can be anything including nulls)               */
/*  arg5 = length of input data to be converted                       */
/*  arg6 = address of EBCDIC to ASCII translate table                 */
/*  arg7 = control bits                                               */
/*         0x00 - does not terminate output areas                     */
/*         0x01 - null terminates output areas                        */
/*         0x02 - translate chars from EBCDIC to ASCII                */
/*                before converting any unprintables to periods       */
/*                                                                    */
/*Apr04/06 - change '.' to '^' to represent unprintable characters    */
/*Nov14/07 - change '^' back to '.'                                   */

void data2vhx(char *chars,char *zons,char *digs,char *data,int lth
               ,unsigned char *ebc2asc, short bits)
{
int ii;
intU zz,dd;

for (ii=0; ii < lth; ii++)
  {
    chars[ii] = data[ii];                   /* copy data to chars out     */
    if (bits & 0x02)                         /* translate EBCDIC to ASCII? */
        chars[ii] = ebc2asc[(Uchar)chars[ii]];   

    if (chars[ii] < ' ' || chars[ii] > '~') /* if unprintable ?       */
        chars[ii] = '.';                    /* - convert to periods   */

    zz = (intU)(data[ii] >> 4);             /* shift zone to digit of w/s   */
    zz = zz & 0x0f;                         /* ensure zone bits clear       */
    if (zz < 10)                            /* if binary value 0 to 9       */
        zons[ii] = (char)(zz + 0x30);       /* add 0x30 to get ascii value  */
    else                                    /* if binary value a to f       */
        zons[ii] = (char)(zz + 0x37);       /* add 0x37 to get ascii value  */
                                            /* ex: 0x0a + 0x37 = 0x41 = 'A' */

    dd = (intU)(data[ii] & 0x0f);           /* remove zone of digit rep     */
    if (dd < 10)                            /* if binary value 0 to 9       */
        digs[ii] = (char)(dd + 0x30);       /* add 0x30 to get ascii value  */
    else                                    /* if binary value a to f       */
        digs[ii] = (char)(dd + 0x37);       /* add 0x37 to get ascii value  */
                                            /* ex: 0x0a + 0x37 = 0x41 = 'A' */
  }
if (bits & 0x01)
  { chars[ii] = '\0';                       /* null terminate char data     */
    zons[ii] = '\0';                        /* null terminate zones         */
    digs[ii] = '\0';                        /* null terminate digits        */
  }
return;
}
 
/*eject*/
/*uvhdcob-------------------- data2hex -------------------------------*/
/* data2hex.c - function to convert data to hex representation        */
/*            - by Owen Townsend - ARC - aug 92                       */
/*Feb02/09 - add options to suppress trailing blanks (ASCII or EBCDIC)*/
/* - arg1 is the output data area (should be 2 times input length)    */
/* - arg2 is the input data (can be anything including nulls)         */
/* - arg3 is the length of the output area (2x input data)            */
/* - arg4 is bit codes modifying operation                            */
/*           bit 0x02 - null terminate the output area                */
/*           bit 0x04 - suppress trailing ASCII blanks                */
/*           bit 0x08 - suppress trailing EBCDIC blanks               */

void data2hex(char *out, char *in, int outlth, short bits)
{
int ii,jj;
unsigned int zz,dd;                       /* work areas for zone & digit */

for (ii=0,jj=0; jj < outlth; ii++,jj++,jj++)
  {
    zz = (unsigned int)(in[ii] >> 4);     /* shift zone to digit of w/s   */
    zz = zz & 0x0f;                       /* ensure zone bits clear       */
    if (zz < 10)                          /* if binary value 0 to 9       */
        out[jj] = (char)(zz + 0x30);      /* add 0x30 to get ascii value  */
    else                                  /* if binary value a to f       */
        out[jj] = (char)(zz + 0x37);      /* add 0x37 to get ascii value  */
                                          /* ex: 0x0a + 0x37 = 0x41 = 'A' */

    dd = (unsigned int)(in[ii] & 0x0f);   /* remove zone of digit rep     */
    if (dd < 10)                          /* if binary value 0 to 9       */
        out[jj+1] = (char)(dd + 0x30);    /* add 0x30 to get ascii value  */
    else                                  /* if binary value a to f       */
        out[jj+1] = (char)(dd + 0x37);    /* add 0x37 to get ascii value  */
                                          /* ex: 0x0a + 0x37 = 0x41 = 'A' */
  }

/*eject*/
/* test option to suppress trailing ASCII blanks */
if (bits & 0x04)
  { for (jj -= 2; jj > 3; jj -= 2)
     { if (memcmp(out+jj,"20",2) == 0)
         { memset(out+jj,' ',2);
         }
       else
         { jj += 2;
           break;
         }
     }
  }

/* test option to suppress trailing EBCDIC blanks */
if (bits & 0x08)
  { for (jj -= 2; jj > 3; jj -= 2)
     { if (memcmp(out+jj,"40",2) == 0)
         { memset(out+jj,' ',2);
         }
       else
         { jj += 2;
           break;
         }
     }
  }

if (bits & 0x02)
    out[jj] = '\0';                       /* null terminate the output    */
return;
}
/*------------------------- end of uvhd.c program ---------------------*/
