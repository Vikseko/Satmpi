import glob
import os


input1 = "/home/vsk/Satmpi/Main_track2/original/"
input2 = "/home/vsk/Satmpi/Main_track2/repeats/"
input3 = "/home/vsk/Satmpi/Main_track2/modified/"

def isInteger(s):
    try: 
        int(s)
        return True
    except ValueError:
        return False

def append_learnts(
    orig_cnf_fn,
    repeats_fn, 
    modfile_fn, 
    portion=1,
    size_lim=0,
    repeats_lim = 0
    ):
    nVars = 0
    nClauses = 0
    cnf = []
    nCl = 0
    with open(orig_cnf_fn,"r") as infile:
        for line in infile:
            if line.startswith("p cnf"):
                t = line.split(" ")
                nVars = int(t[2])
                nClauses = int(t[3])
            else:
                cnf.append(line.strip(" \r\n"))
    
    duplicate_learnts = []
    with open(repeats_fn,"r") as infile:
        for line in infile:
            repeats_cnt = 0            
            if line.startswith("c"):                
                p = line.split(" ")
                repeats_cnt = int (p[1])
                duplicate_learnts [-1][1] = repeats_cnt
            else:
                line=line.strip("\r\n ")
                p = line.split(" ")
                t = [int (u) for u in p if u !="0"]
                duplicate_learnts.append([t,0])
    
    #need to put everything into one list first to get the 
    #correct header
    limit = round(portion * nClauses)
    k_added = 0
    for u in duplicate_learnts:
        if k_added > limit:
            break        
        add = True
        if size_lim>0:
            if len(u[0])>size_lim:
                add = False
        if repeats_lim>0:
            if u[1]<repeats_lim:
                add = False
        if add == True:
            cnf.append("c repeated {} ".format(u[1]))
            cnf.append(" ".join([str(v) for v in u[0]])+" 0")
            k_added += 1
    
    with open(modfile_fn,"w") as outfile:
        outfile.write("p cnf {} {}\r\n".format(nVars,nClauses+k_added))
        for u in cnf:
            outfile.write(u+"\r\n")

def find_files(path_original, path_repeats, path_modified):    
    
    orig_files = glob.glob(path_original+"*.cnf")
    orig_filenames = [u.split("/")[-1] for u in orig_files]

    repeats_files = glob.glob(path_repeats+"*.cnf")
    repeats_filenames = [u.split("/")[-1] for u in repeats_files]

    for u in orig_filenames:
        if ("hash"+u) in repeats_filenames:
            append_learnts(
                path_original+u,
                path_repeats+"hash"+u,
                path_modified+"mod_"+u)
    

    

find_files(input1,input2,input3)
#do_stuff(Geffe250_ursapath)
#do_stuff(Geffe_ursapath)
#do_stuff(Gifford_ursapath)
#do_stuff(Bivium_ursapath)
#do_stuff(Trivium_ursapath)
#do_stuff(Grain_ursapath)





