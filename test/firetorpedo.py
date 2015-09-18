import libpyAI as ai

count = 0

def AI_loop():
    global count
    count += 1
    if count % 10 == 0:
        print ("SHIELD", ai.selfShield())
        if ai.selfShield():
            ai.shield()
        ai.fireTorpedo()

ai.start(AI_loop,["-name","FireTorpedo", "-join"])
