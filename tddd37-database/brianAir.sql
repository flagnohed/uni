SET FOREIGN_KEY_CHECKS = 0;

drop table if exists airport;
drop table if exists route;
drop table if exists weekly_schedule;
drop table if exists year_table;
drop table if exists week_day;
drop table if exists flight;
drop table if exists ticket;
drop table if exists passenger;
drop table if exists reservation;
drop table if exists payment;
drop table if exists credit_info;
drop table if exists booking;
drop table if exists contact;

drop procedure if exists addYear;
drop procedure if exists addDay;
drop procedure if exists addDestination;
drop procedure if exists addRoute;
drop procedure if exists addFlight;
drop procedure if exists addReservation;
drop procedure if exists addPassenger;
drop procedure if exists addContact;
drop procedure if exists addPayment;

drop view if exists allFlights;

drop function if exists calculateFreeSeats;
drop function if exists calculatePrice;

drop trigger if exists generateSecretKey;

# creating tables
create table airport(
	a_code varchar(3),
    a_name varchar(30),
    country varchar(30),
    primary key(a_code)
);

create table weekly_schedule(
	id int not null auto_increment,
    dep_time time,
    ws_day varchar(10),
    ws_year int,
    ws_route int,
    primary key(id)
);

create table year_table(
	id int,
    profit_factor double,
    primary key(id)
);

create table route(
	id int not null auto_increment,
    arrival_code varchar(3),
    departure_code varchar(3),
    route_price double,
    ws_id int,
    year_id int,
    primary key(id),
    foreign key(ws_id) references weekly_schedule(id),
    foreign key(year_id) references year_table(id)
);

create table week_day(
	w_day varchar(10),
    weekday_factor double,
    year_id int,
    primary key(w_day),
    foreign key(year_id) references year_table(id)

);

create table flight(
	flight_number int not null auto_increment,
    fweek int,
    ws_id int,
    route_id int,
    primary key(flight_number),
    foreign key(ws_id) references weekly_schedule(id),
    foreign key(route_id) references route(id)
);

create table contact(
	id int not null auto_increment,
    email varchar(30),
    phone_num bigint,
    is_passenger bool,
    primary key(id)
);

create table passenger(
	id int not null auto_increment,
	pass_num int,
    full_name varchar(256),
    res_nr int,
    primary key(id),
    foreign key(res_nr) references reservation(id) on delete cascade
);

create table credit_info(
	id int not null auto_increment,
	card_num bigint,
    holder varchar(256),
    primary key(id)

);

create table payment(
	id int not null auto_increment,
    credit_id int,
    total_price double,
    primary key(id),
    foreign key(credit_id) references credit_info(id)
);

create table booking(
	id int not null auto_increment,
    contact_id int,
    passenger_id int,
    pay_id int,
    res_id int,
    num_passengers int,
    flight_id int,
    primary key(id),
    foreign key(contact_id) references contact(id),
    foreign key(passenger_id) references passenger(id),
    foreign key(pay_id) references payment(id),
    foreign key(res_id) references reservation(id),
    foreign key(flight_id) references flight(flight_number)
);

create table ticket(
	id int not null auto_increment,
	secret_key int,
    flight_id int,
    booking_id int,
    primary key(id),
    foreign key(flight_id) references flight(flight_number),
    foreign key(booking_id) references booking(id)
);

create table reservation(
	id int not null auto_increment,
	res_num int,
    num_passengers int unsigned not null,
    flight_id int,
    contact_id int,
    is_booking bool,
    primary key(id),
    foreign key(flight_id) references flight(flight_number),
    foreign key(contact_id) references contact(id)
);

SET FOREIGN_KEY_CHECKS = 1;

#creating procedures
delimiter //

create procedure addYear(in in_year int, in factor double)
begin
insert into year_table (id, profit_factor) values (in_year, factor);
end;//

create procedure addDay(in in_year int, in week_day varchar(10), in factor double)
begin
insert into week_day(year_id, w_day, weekday_factor) values(in_year, week_day, factor);
end;//

create procedure addDestination(in airport_code varchar(3), in in_name varchar(30), in country varchar(30))
begin
insert into airport(a_name, a_code, country) values (in_name, airport_code, country);
end;//

create procedure addRoute(in dep_airport_code varchar(3), in arr_airport_code varchar(3), in in_year int, in routeprice double)
begin
declare wsid int;
select id into wsid from weekly_schedule where weekly_schedule.ws_year = in_year;
insert into route(departure_code, arrival_code, year_id, route_price, ws_id) values (dep_airport_code, arr_airport_code, in_year, routeprice, wsid);
end;//

create procedure addFlight(in dep_airport_code varchar(3), in arr_airport_code varchar(3), in in_year int, in in_day varchar(10), in dep_time time)
begin
declare route_id int;
declare count int;
declare ws_id int;
select id into route_id from route where route.arrival_code = arr_airport_code and route.departure_code = dep_airport_code and route.year_id = in_year;

insert into weekly_schedule (dep_time, ws_day, ws_year, ws_route) values (dep_time, in_day, in_year, route_id);
set count = 1;
set ws_id = last_insert_id();
update route set route.ws_id = ws_id where route.id = route_id;

while count <= 52 do
insert into flight (flight.fweek, flight.ws_id, flight.route_id) values (count, ws_id, route_id);
set count = count + 1;
end while;
end;//

create function calculateFreeSeats(flight_number int)
returns int
begin
declare num_booked int;

select count(*) into num_booked from ticket where flight_id = flight_number;
return 40-num_booked;
end;//

create function calculatePrice(flightnumber int)
returns double
begin
declare num_booked int;
declare routeprice double;
declare profitfactor double;
declare weekdayfactor double;
declare wsid int;
declare total_price double;
declare routeid int;
declare tmp_year int;
declare tmp_day varchar(10);
declare resnum int;


select ifnull(sum(reservation.num_passengers),0) into num_booked from reservation where reservation.flight_id = flightnumber;
select ws_id into wsid from flight where flight_number = flightnumber;
select flight.route_id into routeid from flight where flight.flight_number = flightnumber;
select route_price into routeprice from route where route.id = (select weekly_schedule.ws_route from weekly_schedule where weekly_schedule.id = wsid);
select ws_year into tmp_year from weekly_schedule where weekly_schedule.id = wsid;
select profit_factor into profitfactor from year_table where year_table.id = tmp_year;
select ws_day into tmp_day from weekly_schedule where weekly_schedule.id = wsid;
select weekday_factor into weekdayfactor from week_day where tmp_day = week_day.w_day and tmp_year = week_day.year_id;

set total_price = routeprice * weekdayfactor * ((num_booked + 1)/40) * profitfactor;
return round(total_price,3);
end;//

create trigger generateSecretKey
after insert on booking
for each row
begin
	declare s_key int;
	declare num_pass int;
    select num_passengers into num_pass from reservation where id = new.res_id;
    repeat
		set s_key = rand();
		insert into ticket (secret_key, flight_id) values (s_key, (select flight_id from reservation where id = new.res_id));
		set num_pass = num_pass - 1;
    until num_pass = 0
    end repeat;
end;//

create procedure addReservation(in dep_airport_code varchar(3), in arr_airport_code varchar(3), in in_year int, in in_week int, in in_day varchar(10), in in_time time, in num_passengers int, out out_res_nr int)
begin
declare curr_flight int;
declare free_seats int;
declare curr_route int;
declare wsid int;
declare out_res_id int;

select id into curr_route from route where route.arrival_code = arr_airport_code and route.departure_code = dep_airport_code and route.year_id = in_year;
select id into wsid from weekly_schedule where weekly_schedule.ws_route = curr_route and weekly_schedule.ws_day = in_day and weekly_schedule.dep_time = in_time and weekly_schedule.ws_year = in_year;
select flight_number into curr_flight from flight where flight.route_id = curr_route and flight.ws_id = wsid and flight.fweek = in_week;

set free_seats = calculateFreeSeats(curr_flight);
if curr_flight is not null then
	if num_passengers <= free_seats then
		insert into reservation (flight_id, num_passengers, is_booking) values (curr_flight, num_passengers, False);
		set out_res_id = last_insert_id();
		update reservation set res_num = out_res_id where id = out_res_id;
		select res_num into out_res_nr from reservation where id = out_res_id;
	else
		select "There are not enough seats available on the chosen flight" as "";
	end if;
else
	select "There exist no flight for the given route, date and time" as "";
end if;
end;//

create procedure addPassenger(in res_nr int, in pass_num int, in in_name varchar(256))
begin
declare curr_res int;
declare resid int;
declare bool_booked bool;

select id into resid from reservation where res_num = res_nr;
if resid is not null then
	select is_booking into bool_booked from reservation where res_num = resid;
    if not bool_booked then
		insert into passenger(pass_num, full_name, res_nr) values(pass_num, in_name, resid);
	else
		select "The booking has already been payed and no futher passengers can be added" as "";
    end if;
else
	select "The given reservation number does not exist" as "";
end if;
end;//

create procedure addContact(in res_nr int, in pass_num int, in email varchar(30), in phone_num bigint)
begin
declare curr_pass int;
declare res_num int;

if exists (select passenger.pass_num from passenger where passenger.pass_num = pass_num)then
	insert into contact (email, phone_num, is_passenger) values (email, phone_num, True);
    select reservation.id into res_num from reservation where reservation.id = res_nr;
    if res_num is not null then
		update reservation set contact_id = last_insert_id() where id = res_nr;
	else
		select "The given reservation number does not exist" as "";
    end if;
else
	select "The person is not a passenger of the reservation" as "";
end if;
end;//

create procedure addPayment(in res_nr int, in cardholder_name varchar(256), in credit_card_num bigint)
begin
declare flight_num int;
declare num_pass int;
declare free_seats int;
declare creditid int;
declare totalprice int;
declare payid int;
declare contactid int;
declare bookingid int;
declare resnum int;

select id into resnum from reservation where id = res_nr;
if resnum is not null then
	select contact_id into contactid from reservation where res_num = resnum;
	if contactid is not null then
		select flight_id into flight_num from reservation where res_num = resnum;
		if flight_num is not null then
			select count(*) into num_pass from passenger where res_nr = resnum;
			set free_seats = calculateFreeSeats(flight_num);
			if free_seats >= num_pass then
				insert into credit_info (holder, card_num) values (cardholder_name, credit_card_num);

				set creditid = last_insert_id();
				set totalprice = calculatePrice(flight_num);
				insert into payment (credit_id, total_price) values (creditid, totalprice);

				set payid = last_insert_id();
				update reservation set reservation.is_booking = True where res_num = resnum;
				insert into booking (contact_id, passenger_id, pay_id, res_id, flight_id) values (contactid, contactid, payid, resnum, flight_num);

				set bookingid = last_insert_id();
				update ticket set ticket.booking_id = bookingid where ticket.flight_id = flight_num;
			else
				select 'There are not enough seats available on the flight anymore, deleting reservation' as '';
                delete from reservation where id = resnum;
			end if;
		else
			select "There exist no flight for the given route, date and time" as "";
		end if;
	else
		select 'Contact not found' as '';
	end if;
else
	select "The given reservation number does not exist" as "";
end if;
end;//




create view allFlights (
	departure_city_name,
    destination_city_name,
    departure_time,
    departure_day,
    departure_week,
    departure_year,
    nr_of_free_seats,
    current_price_per_seat
)
as
select
	dep.a_name,
    arr.a_name,
    ws.dep_time,
    ws.ws_day, 
    fl.fweek,
    ws.ws_year,
    calculateFreeSeats(fl.flight_number),
    calculatePrice(fl.flight_number)
from flight as fl
inner join weekly_schedule as ws on fl.ws_id = ws.id
inner join airport as dep on dep.a_code = (select departure_code from route where id = ws.ws_route)
inner join airport as arr on arr.a_code = (select arrival_code from route where id = ws.ws_route);
