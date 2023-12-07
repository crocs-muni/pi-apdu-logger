from bitcoinlib.transactions import Transaction

# Create a new transaction
tx = Transaction()

# Define input transaction details
tx.add_input(prev_txid='eb6c6f0c99b2996f2cd28cb4a6da07d82b1bf9b89ddc7ef6e05b5f4d3c24e3fe', output_n=1)

# Define output transaction details
output_address = '1A1zP1eP5QGefi2DMPTfTL5SLmv7DivfNa'  # Example Bitcoin address
output_amount = 2  # BTC
tx.add_output(value=output_amount, address=output_address)

# Print the constructed transaction details
tx.info()