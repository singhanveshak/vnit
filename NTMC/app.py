#Runs on Charm Cypto 
#This is an implementation of CL-AKA protocol given by Xia, Zhang https://link.springer.com/article/10.1007/s12243-025-01108-x

from charm.toolbox.pairinggroup import PairingGroup, ZR, G1
import hashlib
import secrets
import time

group = PairingGroup('MNT224')

def H1(ID: str, p1, p2):
    data = b"".join([ID.encode(), group.serialize(p1), group.serialize(p2)])
    return group.hash(data, ZR)

def H2(X_i, X_j, l_point):
    data = b"".join([group.serialize(X_i), group.serialize(X_j), group.serialize(l_point)])
    return group.hash(data, ZR)

def H3(*parts):
    h = hashlib.sha256()
    for p in parts:
        if p is None:
            continue
        if isinstance(p, str):
            h.update(p.encode())
        else:
            h.update(group.serialize(p))
    digest = h.digest()
    return group.hash(digest, ZR)

class KGC:
    def __init__(self, group):
        self.group = group
        self.s = group.random(ZR)
        self.P = group.random(G1)
        self.Ppub = self.s * self.P

    def issue_partial(self, ID_E: str, X_E):
        r_E = self.group.random(ZR)
        R_E = r_E * self.P
        h1 = H1(ID_E, X_E, R_E)
        y_E = r_E + (h1 * self.s)
        return y_E, R_E

class Entity:
    def __init__(self, ID_E: str, kgc: KGC):
        self.ID_E = ID_E
        self.kgc = kgc
        self.x_E = group.random(ZR)
        self.X_E = self.x_E * self.kgc.P
        self.y_E = None
        self.R_E = None
        self.t_E = None
        self.T_E = None
        self.ts_E = None

    def register_with_kgc(self):
        y_E, R_E = self.kgc.issue_partial(self.ID_E, self.X_E)
        lhs = y_E * self.kgc.P
        h1 = H1(self.ID_E, self.X_E, R_E)
        rhs = R_E + (h1 * self.kgc.Ppub)
        if self.kgc.group.serialize(lhs) != self.kgc.group.serialize(rhs):
            return False
        self.y_E = y_E
        self.R_E = R_E
        return True

    def make_auth_request(self):
        self.t_E = group.random(ZR)
        self.T_E = self.t_E * self.kgc.P
        self.ts_E = str(secrets.randbits(64))
        return (self.ID_E, self.X_E, self.R_E, self.T_E, self.ts_E)

    def respond_to_auth(self, incoming_msg):
        (ID_peer, X_peer, R_peer, T_peer, ts_peer) = incoming_msg
        self.t_E = group.random(ZR)
        self.T_E = self.t_E * self.kgc.P
        h1 = H1(ID_peer, X_peer, R_peer)
        inner = R_peer + (h1 * self.kgc.Ppub)
        l_E = self.y_E * inner
        h_E = H2(self.X_E, X_peer, l_E)
        scalar_multiplier = h_E * (self.t_E + self.x_E)
        point_term = T_peer + X_peer
        K_E = scalar_multiplier * point_term
        SK_E = H3(ID_peer, self.ID_E, X_peer, self.X_E, R_peer, self.R_E, T_peer, self.T_E, K_E)
        resp_msg = (self.ID_E, self.X_E, self.R_E, self.T_E, str(secrets.randbits(64)))
        return resp_msg, SK_E

    def finish_auth(self, incoming_msg):
        (ID_peer, X_peer, R_peer, T_peer, ts_peer) = incoming_msg
        h1 = H1(ID_peer, X_peer, R_peer)
        inner = R_peer + (h1 * self.kgc.Ppub)
        l_E = self.y_E * inner
        h_E = H2(X_peer, self.X_E, l_E)
        scalar_multiplier = h_E * (self.t_E + self.x_E)
        point_term = T_peer + X_peer
        K_E = scalar_multiplier * point_term
        SK_E = H3(self.ID_E, ID_peer, self.X_E, X_peer, self.R_E, R_peer, self.T_E, T_peer, K_E)
        return SK_E

def simulate():
    print("Initializing KGC and entities...")
    kgc = KGC(group)
    SD = Entity("device01", kgc)
    CS = Entity("cloud01", kgc)
    ok_sd = SD.register_with_kgc()
    ok_cs = CS.register_with_kgc()
    if ok_sd and ok_cs:
        print("Registration Phase Done")
    print("\n--- Authentication phase (SD -> CS) ---")
    msg1 = SD.make_auth_request()
    msg2, SK_CS = CS.respond_to_auth(msg1)
    SK_SD = SD.finish_auth(msg2)
    print ("Authentication Done")
    
    SK_CS_bytes = hashlib.sha256(group.serialize(SK_CS)).digest()
    SK_SD_bytes = hashlib.sha256(group.serialize(SK_SD)).digest()
    
    print("SK_CS (hex):", SK_CS_bytes.hex())
    print("SK_SD (hex):", SK_SD_bytes.hex())
    
    if SK_SD_bytes == SK_SD_bytes:
        print("Shared Session Key : ", SK_SD_bytes.hex())
		

if __name__ == "__main__":
    simulate()
    
    N = 1000

    a = group.random(ZR)
    P = group.random(G1)
    t0 = time.time()
    for _ in range(N):
       _ = a * P
    T_SM = (time.time() - t0) / N

    A = group.random(G1); B = group.random(G1)
    t0 = time.time()
    for _ in range(N):
        _ = A + B
    T_PA = (time.time() - t0) / N

    t0 = time.time()
    for _ in range(N):
        _ = group.hash(b"test", ZR)
    T_H = (time.time() - t0) / N

    print(f"T_SM = {T_SM}\nT_PA = {T_PA}\nT_H = {T_H}" )
    
    # Registration cost per entity
    C_reg = 4*T_SM + 1*T_PA + 2*T_H

    # Authentication cost per entity
    C_auth_entity = 4*T_SM + 2*T_PA + 3*T_H

    # Authentication total (two entities)
    C_auth_total = 2 * C_auth_entity

    print(f"Registration (per entity): {C_reg*1000:.3f} ms")
    print(f"Authentication (per entity): {C_auth_entity*1000:.3f} ms")
    print(f"Authentication total: {C_auth_total*1000:.3f} ms")
