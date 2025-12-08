(* Day 2 : gift Shop *)

(** input_file name is read from argv *)
let input_file = 
  if Array.length Sys.argv < 2 then 
    failwith ("Usage : " ^ Sys.argv.(0) ^ " input_file")
  else
    Sys.argv.(1)


let read_input () =
  let f = open_in input_file in
  let input = 
    input_line f
    |> String.split_on_char ','
    |> List.map (fun s -> Scanf.sscanf s "%d-%d" (fun a b ->(a,b)))
  in
  close_in f;
  input
  


(* Part 1 *)

(* The smart solution is to observe that all squares are divisible by 100..01 
  with a good number of 0; and then use the sum of integers.
  But today, I'm not smart. *)
let is_square x =
  let s = string_of_int x in
  let n = String.length s in
  if n mod 2 = 1 then 
    false 
  else
    try for i = 0 to n/2-1 do if s.[i] <> s.[n/2+i] then raise Exit done; true
    with Exit -> false


let part1 input =
  let sum_in_range a b =
    Seq.init (b-a+1) (fun i -> a+i)
    |> Seq.fold_left (fun s x -> if is_square x then s+x else s) 0 
  in
  List.fold_left (fun s (a,b) -> s + sum_in_range a b) 0 input



(* Part 2 *)

(** Checks if string s has period p *)
let has_period p s =
  let n = String.length s in
  if n mod p <> 0 then
    false
  else
    try
      (* checks if all p*i +j are equals *)
      for i = 1 to n/p -1 do
        for j = 1 to p-1 do
          if s.[j] <> s.[i*p +j] then begin raise Exit end
        done
      done;
      true;
    with
      Exit -> false

let is_periodic x =
  let s = string_of_int x in
  let n = String.length s in

  let exception Perio in
  let exception Next_p in
  try
    for p = 1 to n/2 do
      try 
        if n mod p <> 0 then raise Next_p;
        for i = 0 to n-p-1 do if s.[i] <> s.[i+p] then raise Next_p done;
        raise Perio
      with 
        Next_p -> () (* continue *)
    done;
    false
  with
    Perio -> true

let part2 input =
  let sum_in_range a b =
    Seq.init (b-a+1) (fun i -> a+i)
    |> Seq.fold_left (fun s x -> if is_periodic x then s+x else s) 0 
  in
  List.fold_left (fun s (a,b) -> s + sum_in_range a b) 0 input
  


(** main *)
let () =
  (* has_period 2 ""
  |> Printf.printf "%b\n" *)
  (* sum_in_range 1188511880 1188511890
  |> Printf.printf "%d\n" *)
  let input = read_input () in
  Printf.printf "%d\n%d\n" (part1 input) (part2 input)