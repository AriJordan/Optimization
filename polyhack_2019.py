# This code won our team the contest with 1000 CHF prize money

from data import * # func, var and libs
level = 3
dem, veh, nodes, route = get_data(level)
dem = list(dem.values())

from collections import defaultdict
ev_q = defaultdict(list)

T_MAX = 1
RES = 2

revenue = 0.
def init(level):
	global T_MAX, BS
	if level == 1:
		ev_q[0] = [('Drone1','A'),('Drone2','C'),('Drone3','C'),('Drone4','B')]
		T_MAX = 2 * 60 * RES
		BS = 60 * RES
	if level == 2:
		ev_q[0] = [('Drone1','B'),('Drone2','E'),('Drone3','C'),('Drone4','A'),('Drone5','E'),('Drone6','D')]
		T_MAX = 6 * 60 * RES
		BS = 30 * RES
	if level == 3:
		ev_q[0] = [('Drone1','G'),('Drone2','B'),('Drone3','F'),('Drone4','E'),('Drone5','B'),('Drone6','B'),('Drone7','A'),('Drone8','E'),('Drone9','C')]
		T_MAX = 6 * 60 * RES
		BS = 30 * RES
init(level)

ports = pd.concat([nodes for _ in range(T_MAX + BS)], keys=range(T_MAX + BS))
ports.index.names = 't', 'id'
# ports.loc[t]
# ports.loc[t,id]
# ports.loc[t,id].ncor or .ngat

def rev(npax, vid, src, dest):
    for dst, rev, dist, min_time, max_time in route[src]:
        if dest == dst:
            # print(npax,rev,min_time/RES, veh.loc[vid].cost)
            return npax * rev - min_time * veh.loc[vid].cost /60/ RES

def check(npax, src, dest, t1, t2, vid):
    # other things to check, esp at t1
    if npax > veh.loc[vid].Pax_max: return False
    for dst, rev, dist, min_time, max_time in route[src]:
        if dest == dst: break
    else: return False
    for t in range(t2, t2+2):
        if ports.loc[t,dest].ncor < 1:
            return False
    for t in range(t2+6, t2+8):
        if ports.loc[t,dest].ncor < 1:
        	return False
    return True

def do_it(npax, src, dest, t1, t2, vid):
    # print(t1, t1//BS, dem[t1//BS])
    dem[t1//BS][(src,dest)] -= npax
    for t in range(t2, t2+2):
    	ports.loc[t, dest].ncor -= 1
    for t in range(t2+6, t2+8):
    	ports.loc[t, dest].ncor -= 1



def solve():
    for t in range(T_MAX):
        ev_q[t].sort()
        for vid,src in ev_q[t]:
            Pmax, cost = veh.loc[vid]
            bpax, bdest, brev, btim = None, None, -1e9, None
            for dest, rv, dist, min_time, max_time in route[src]:
                # print(1,dest)
                npax = min(Pmax,dem[t//BS][(src,dest)])
                if not check(npax, src, dest, t, t + min_time, vid):
                	continue
                # print(2,dest)
                nrev = rev(npax, vid, src, dest)
                # print("revs:", nrev, brev)
                if nrev > brev:
                    brev = nrev
                    bpax = npax
                    btim = min_time
                    bdest = dest
            if not bdest:
            	ev_q[t+1].append((vid, src))
            	# print('NOOOO', vid, src, t)
            else:
                t2 = t+btim
                do_it(bpax, src, bdest, t, t2, vid)
                log(bpax, src, bdest, vid, t, t2)
                ev_q[t2 + RES*4].append( (vid,bdest) )

LOG = []

def log(npax, src, dest, vid, t1, t2):
    LOG.append((npax, src, dest, vid, t1, t2))
    # LOG.append((npax, src.cor, dest, src.cor, src, id_veh, t1, t2))

def to_json():
    # f = open(fname, 'w')
    df = DF(FINAL_LOG, columns=[
        "Npax",
        "arriveFato",
        "arriveVertiport",
        "departureFato",
        "departureVertiport",
        "flightId",
        "missionId",
        "pavId",
        "sTOT",
        "sLDT",
        ])
    pprint(df.to_dict('records'))
    # print('[')
    # print(*(line.to_json() for i,line in df.iterrows()), sep=',\n', end='\n]\n')

solve()

FINAL_LOG = []


from datetime import datetime, timedelta
epoch = datetime(2019,10,17)
dt    = timedelta(seconds=30)

def cv_time(nt):
    # print (epoch + nt * dt)
    return (epoch + nt * dt).strftime('%Y/%m/%dT%H:%M:%SZ')

def final_log():
    ev = defaultdict( list )
    for (npax, src, dest, vid, t1, t2) in LOG:
        assert(t1<t2)
        ev[t1-RES].append( (vid, src) )
        ev[t2].append( (vid, dest) )
    choices = {}
    CORRS = set()
    for t in range(-RES, T_MAX + BS):
        for vid, pid in ev[t]:
            for cor in range(1,69): # should be enough
                for i in range(RES):
                    k = (t+i, cor, pid)
                    if k in CORRS: break
                else:
                    for i in range(RES):
                        k = (t+i, cor, pid)
                        CORRS.add(k)
                    choices[(t,vid)] = cor
                    break
    for i,(npax, src, dest, vid, t1, t2) in enumerate(LOG):
        c1,c2 = choices[(t1-RES,vid)],choices[(t2,vid)]
        # print(t1,t2,cv_time(t1),cv_time(t2))
        assert(t1<t2)
        FINAL_LOG.append((int(npax), c2, dest, c1, src,
            i,i,
            vid,
            cv_time(t1),
            cv_time(t2),
            ))

final_log()
to_json()
