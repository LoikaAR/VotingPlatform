export default function Host() {
    return (
        <div>
            <main>
                <h1>
                    Host a vote
                </h1>
            
                <div className="host_form flex flex-col items-center gap-4">
                    <h2>Vote set up:</h2>
                    <textarea rows={2} cols={20} placeholder={"The title of your vote"}></textarea>
                    <textarea rows={5} cols={30} placeholder={"Description"}></textarea>
                </div>
            </main>
        </div>
    );
}