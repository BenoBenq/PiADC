nameList = ["ServerSocket", "ClientSocket", "ClientStartThread", "ServerStartThread", "ClientRecievingThread", "ClientSendingThread", "ServerSendingThread", "SharedMemoryStruc", "ServerRecievingThread", "SPISetting", "ReadSPIdevice"]
fileList = ["pc.c", "pi.c", "dataAq.c"]

out = open("snipets_data.dat", "w")

for file in fileList:
    f = open("../" + file, "r")
    line_number = 1
    for l in f:
        for name in nameList:
            for suffix in ['B', 'E']:
                if "//#" + name + suffix in l and suffix == 'B':
                    print(name + suffix + " = " + str(line_number + 1), file=out)
                elif "//#" + name + suffix in l and suffix == 'E':
                    print(name + suffix + " = " + str(line_number - 1), file=out)
        line_number += 1

    f.close()
