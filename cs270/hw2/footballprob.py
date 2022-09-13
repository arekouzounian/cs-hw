import itertools 
import random 

def func(b, r): 
    b.sort()
    r.sort()

    matches = {}

    for i in range(len(b)):
        back = i
        if back not in matches: 
            for j in range(len(r)): 
                if j not in matches.values():
                    matches[back] = j 
                    break
        else:
            for i in range(matches[back], len(r)):
                if r[i] < back:
                    matches[back] = i

    lst = []
    for match in matches: 
        lst.append((b[match], r[matches[match]]))

    return lst  

def gain_check(pairs): 
    sum = 0
    for pair in pairs:
        sum += gain_single(pair[0], pair[1])
    
    return sum 

def gain_single(b, r):
    oper = (1 + ((r - b) ** 2))
    if b < r: 
        return oper 
    else:
        return 1 / oper


def find_gain(b, r):
    pairs = list(itertools.product(b, r))
    for pair in pairs:
        print("Pair:", pair, "; Gain:", gain_single(pair[0], pair[1]))


def gen_pairs(lst1, lst2):
    if len(lst1) != len(lst2):
        print('unequal lengths! error!')
        return 
    lsts = []
    for offset in range(len(lst1)):
        lst = []
        for i in range(len(lst2)):
            lst.append((lst1[i], lst2[(i + offset) % len(lst2)]))
        lsts.append(lst)

    return lsts 

def find_opt(b, r):
    pairs = gen_pairs(b, r)
    smallest = 10000000000.0
    for lst in pairs:
        smallest = min(smallest, gain_check(lst))

    return smallest 



def main(): 
    backs = [12, 10, 11, 10, 15]
    recvr = [9, 15, 13, 12, 10]
    

    n = 10

    rand1 = [random.randint(1, 20) for _ in range(n)]
    rand2 = [random.randint(1, 20) for _ in range(n)]
    
    ret = func(rand1, rand2)
    print("Returned list:", ret)
    print("Sum gain:", gain_check(ret))

    print("Optimal gain:", find_opt(rand1, rand2))

    #find_gain(rand1, rand2)


if __name__ == '__main__':
    main()