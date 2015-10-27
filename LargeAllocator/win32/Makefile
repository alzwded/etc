.SUFFIXES:.cpp .hxx .h .obj

OBJS = MM.obj FileUtils_win32_msvc.obj

test_MM.exe: $(OBJS)
	link.exe /OUT:test_MM.exe /DEBUG /PDB:test_MM.pdb $(OBJS)

{..\}.cpp{}.obj::
	cl.exe /Zi /EHa /I.. /DMM_TEST /c $<

clean:
	del /q test_MM.exe $(OBJS) *.tmp *.ilk *.pdb
