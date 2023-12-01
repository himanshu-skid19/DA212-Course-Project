
output: main.o authentication.o user_dashboard.o leave_sys.o run.o
	g++ main.o authentication.o user_dashboard.o leave_sys.o run.o -o output -lssl -lcrypto

main.o: main.cpp
	g++ -c main.cpp

authentication.o: authentication.cpp authentication.h
	g++ -c authentication.cpp -lssl -lcrypto

user_dashboard.o: user_dashboard.cpp user_dashboard.h
	g++ -c user_dashboard.cpp

leave_sys.o: leave_sys.cpp leave_sys.h
	g++ -c leave_sys.cpp

run.o: run.cpp run.h
	g++ -c run.cpp


clean:
	rm *.o output
