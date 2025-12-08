(* Day X : title *)

(** input_file name is read from argv *)
let input_file = 
  if Array.length Sys.argv < 2 then 
    failwith ("Usage : " ^ Sys.argv.(0) ^ " input_file")
  else
    Sys.argv.(1)

let read_input () =
	let rot_of_string s = Scanf.sscanf s " %c%d" (fun c d -> c,d) in
	let f = open_in input_file in
	let input = In_channel.input_lines f |> List.map rot_of_string in
	close_in f;
	input


(* Part 1 *)

(* Explain here if necessary *)

let part1 input =
  List.fold_left (fun (pos, score) (c,n) ->
		let new_pos = (if c = 'R' then pos+n else pos-n) mod 100 in
		(new_pos, score + if new_pos = 0 then 1 else 0)
	) (50,0) input
	|> snd



(* Part 2 *)

(* We go through 0 :
		once for each n/100
		plus maybe one more depending upon the before/after move positions
*)

let part2 input =
  List.fold_left (fun (pos, score) (c,n) ->
		let new_pos = (100 + (if c = 'R' then pos+n else pos-n) mod 100) mod 100 in
		let new_score = 
			score + n / 100 +
			(if c = 'R' && (n mod 100) >= 100 - pos && pos != 0
			 || c = 'L' && (n mod 100) >= pos && pos != 0
			 then 1 else 0) in
		(new_pos, new_score)
	) (50,0) input
	|> snd
  
  


(** main *)
let () =
  let input = read_input () in
  Printf.printf "%d\n%d\n" (part1 input) (part2 input)


