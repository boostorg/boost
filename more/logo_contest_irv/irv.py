
import string

f = open("votes.txt")

def load_votes():
    result = []
    f = open("votes.txt")
    for l in f:
        if string.strip(l) == "vote:":
            result.append([])
        else:
            result[-1].append(int(l))

    return result

def count_first_votes(votes):
    result = {}
    for l in votes:
        if l:
            first_vote = l[0]
            if not result.has_key(first_vote):
                result[first_vote] = 1
            else:            
                result[first_vote] = result[first_vote]+1

    return result

def winner(first_votes):
    print "Trying to find a winner"
    total_votes = 0
    for l in first_votes:
        total_votes += first_votes[l]
    print total_votes, "total votes"
    for l in first_votes:
        if first_votes[l] > total_votes/2:
            return l
    return None

def looser(first_votes):
    min_votes = None
    looser = None
    for l in first_votes:
        if min_votes == None or first_votes[l] < min_votes:
            min_votes = first_votes[l]
            looser = l

    return looser

def drop_looser(raw_votes, looser):
    for l in raw_votes:
        if l and l[0] == looser:
            del l[0]
                

raw = load_votes()
print len(raw), "initial votes"
for i in raw:
    print i
while 1:
    counted = count_first_votes(raw)
    print "First votes:"    
    for i in counted:
        print i, " --> ", counted[i]
    w = winner(counted)
    if w is not None:
        print "Winner is", w
        break
    else:
        print "No winner yet"
        l = looser(counted)
        assert l is not None
        print "Dropping", l, "which had", counted[l], "votes"
        drop_looser(raw, l)

        
    
