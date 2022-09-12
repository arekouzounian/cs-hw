import itertools 

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


def main(): 
    backs = [12, 10, 11, 10, 15]
    recvr = [9, 15, 13, 12, 10]
    
    ret = func(backs, recvr)
    print("Returned list:", ret)
    print("Sum gain:", gain_check(ret))

    find_gain(backs, recvr)



if __name__ == '__main__':
    main()