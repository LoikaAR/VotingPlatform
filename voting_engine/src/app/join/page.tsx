export default function Join() {
    return (
        <div>
            <main>
                <h1>
                    Join a vote
                </h1>
                <div className="host_form flex flex-col items-center gap-4">
                    <h2>Enter the vote code:</h2>
                    <textarea rows={1} cols={20} placeholder={""}></textarea>
                </div>
            </main>
        </div>
    );
}